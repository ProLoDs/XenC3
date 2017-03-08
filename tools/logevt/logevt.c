/*
 * logevt.c
 *
 *  Created on: 16.02.2017
 *      Author: bej
 */

#include <sys/poll.h>
#include <xen/xen.h>

#include <xenctrl.h>

#define PERROR(_m, _a...)                                       \
do {                                                            \
    int __saved_errno = errno;                                  \
    fprintf(stderr, "ERROR: " _m " (%d = %s)\n" , ## _a ,       \
            __saved_errno, strerror(__saved_errno));            \
    errno = __saved_errno;                                      \
} while (0)


/***** Compile time configuration of defaults ********************************/

/* sleep for this long (milliseconds) between checking the trace buffers */
#define POLL_SLEEP_MILLIS 100

#define DEFAULT_PTBUF_SIZE 32
/***** The code **************************************************************/

typedef struct settings_st {
    char *outfile;
    unsigned long poll_sleep; /* milliseconds to sleep between polls */
    uint32_t evt_mask;
    unsigned long logbuf_size;
    unsigned long disk_rsvd;
    unsigned long timeout;
    unsigned long memory_buffer;
    uint8_t discard:1,
        disable_tracing:1,
        start_disabled:1;
} settings_t;

struct t_struct {
    const struct t_info *t_info; /* Structure with information about individual buffers */
    struct t_buf **meta;    /* Pointers to trace buffer metadata */
    unsigned char **data;   /* Pointers to trace buffer data areas */
};

settings_t opts;

int interrupted = 0; /* gets set if we get a SIGHUP */

static xc_interface *xc_handle;
static xc_evtchn *xce_handle = NULL;
static int virq_port = -1;
static int outfd = 1;

static void close_handler(int signal)
{
    interrupted = 1;
}

static struct {
    char * buf;
    unsigned long prod, cons, size;
    unsigned long pending_size, pending_prod;
} membuf = { 0 };

#define MEMBUF_INDEX_RESET_THRESHOLD (1<<29)

/* FIXME -- make a power of 2 so we can mask instead. */
#define MEMBUF_POINTER(_i) (membuf.buf + ((_i) % membuf.size))
#define MEMBUF_CONS_INCREMENT(_n)               \
    do {                                        \
        membuf.cons += (_n);                    \
    } while(0)
#define MEMBUF_PROD_SET(_x)                                             \
    do {                                                                \
        if ( (_x) < membuf.prod ) {                                     \
            fprintf(stderr, "%s: INTERNAL_ERROR: prod %lu, trying to set to %lu!\n", \
                    __func__, membuf.prod, (unsigned long)(_x));        \
            exit(1);                                                    \
        }                                                               \
        membuf.prod = (_x);                                             \
        if ( (_x) > MEMBUF_INDEX_RESET_THRESHOLD )                      \
        {                                                               \
            membuf.prod %= membuf.size;                                 \
            membuf.cons %= membuf.size;                                 \
            if( membuf.prod < membuf.cons )                             \
                membuf.prod += membuf.size;                             \
        }                                                               \
    } while(0)


/**
 * event_init - setup to receive the VIRQ_TBUF event
 */
static void event_init(void)
{
    int rc;

    xce_handle = xc_evtchn_open(NULL, 0);
    if (xce_handle == NULL) {
        perror("event channel open");
        exit(EXIT_FAILURE);
    }

    rc = xc_evtchn_bind_virq(xce_handle, VIRQ_LOGBUF);
    if (rc == -1) {
        PERROR("failed to bind to VIRQ port");
        exit(EXIT_FAILURE);
    }
    virq_port = rc;
}

/**
 * wait_for_event_or_timeout - sleep for the specified number of milliseconds,
 *                             or until an VIRQ_TBUF event occurs
 */
static void wait_for_event_or_timeout(unsigned long milliseconds)
{
    int rc;
    struct pollfd fd = { .fd = xc_evtchn_fd(xce_handle),
                         .events = POLLIN | POLLERR };
    int port;

    rc = poll(&fd, 1, milliseconds);
    if (rc == -1) {
        if (errno == EINTR)
            return;
        PERROR("poll exitted with an error");
        exit(EXIT_FAILURE);
    }

    if (rc == 1) {
        port = xc_evtchn_pending(xce_handle);
        if (port == -1) {
            PERROR("failed to read port from evtchn");
            exit(EXIT_FAILURE);
        }
        if (port != virq_port) {
            fprintf(stderr,
                    "unexpected port returned from evtchn (got %d vs expected %d)\n",
                    port, virq_port);
            exit(EXIT_FAILURE);
        }
        rc = xc_evtchn_unmask(xce_handle, port);
        if (rc == -1) {
            PERROR("failed to write port to evtchn");
            exit(EXIT_FAILURE);
        }
    }
}
