#include "common.h"


static int quit = 1;





void
received_message(struct kore_msg *msg, const void *data)
{
    printf("========================= got message from %u (%d bytes): %.*s, mypid: %d\n", msg->src,
        msg->length, msg->length, (const char *)data, getpid());
}


void handle_signal(int sig)
{
    printf("[%s %d] sig:%d, my pid: %d\n", __FUNCTION__, __LINE__, sig, getpid());


    if (sig != 0)
    {
        switch (sig)
        {
            case SIGQUIT:
            case SIGINT:
            case SIGTERM:
                if(quit == 0)
                {
                    kore_stop();
                    quit = 1;
                }
                break;
            default:
                break;
        }
    }


}


int main(int argc, char *argv[])
{
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGTERM, handle_signal);


    if(kore_init(CONFIG_PATH) == 0)
    {
        quit = 0;

        (void)kore_msg_register(MESSAGE_BUTTON_ID, received_message);

        while (quit != 1) {
            kore_worker_wait(0);
            kore_platform_event_wait(100);
            kore_connection_prune(KORE_CONNECTION_PRUNE_DISCONNECT);
        }

        (void)kore_deinit();
    }

    return 0;
}




