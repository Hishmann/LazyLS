#include "pch.h"

#include <poll.h>
#include <unistd.h>
#include <cstring>
#include "objects.h"

void input_handling(bool& running, EventQueue& out_event_queue) {

    struct pollfd my_pollfd;
    my_pollfd.fd = STDIN_FILENO;  // Watch Standard Input (0)
    my_pollfd.events = POLLIN;    // Alert me if there is data to read
    std::string accumulated;

    while (running) {  

        int ret = poll(&my_pollfd, 1, 500); // Poll the buffer and if no input then sleep for timeout (500 ms) 

        if (ret > 0) {

            if (my_pollfd.revents & POLLIN) { // Data is waiting!
                char buf[128];
                memset(buf, 0, sizeof(buf));
                int n = read(STDIN_FILENO, &buf, sizeof(buf)-1); // Read exactly 1 byte
                
                if (n > 0) {
                    buf[n] = '\0'; // Null-terminate the temp buffer
                    accumulated += buf; 
                }
              
                if (!accumulated.empty()) {

                    size_t mouse_end = accumulated.find_first_of("Mm", 3); 

                    if (accumulated.find("\033[<") == 0 && mouse_end != std::string::npos) {
                        int btn, x, y;
                        char mode;

                        if (sscanf(accumulated.c_str() + 3, "%d;%d;%d%c", &btn, &x, &y, &mode) == 4) {
                            Event e;
                            e.type = (mode == 'M') ? EventType::MOUSE_PRESS : EventType::MOUSE_RELEASE;
                            e.mouse.x = x;
                            e.mouse.y = y;
                            e.mouse.button = btn;
                            out_event_queue.push(e);

                            accumulated.erase(0, mouse_end + 1);
                        }

                    } else if (accumulated.find("\033[") == 0) {

                        if (accumulated.size() < 3) continue;;

                        Event e; e.type = EventType::KEY_PRESS;
                        char c = accumulated[2];
                        switch (c) {
                            case 'A':
                                e.keyboard.type = KeyType::UP_ARROW;                            
                                break;
                            case 'B':
                                e.keyboard.type = KeyType::DOWN_ARROW;                            
                                break;
                            case 'C':
                                e.keyboard.type = KeyType::RIGHT_ARROW;
                                break;
                            case 'D':
                                e.keyboard.type = KeyType::LEFT_ARROW;                            
                                break;
                            case 'E':
                                e.keyboard.type = KeyType::HOME;                            
                                break;
                            case 'F':
                                e.keyboard.type = KeyType::END;                            
                                break;
                            default:
                                e.keyboard.type = KeyType::CHARACTER;
                                break;
                        }
                        e.keyboard.key = 0; e.keyboard.modifiers = KeyModifiers::NONE;
                        out_event_queue.push(e);

                        accumulated.erase(0, 3);

                    } else if (accumulated[0] == 'q') {

                        Event e; e.type = EventType::QUIT;
                        out_event_queue.push(e);
                        running = false;

                        accumulated.erase(0, 1);

                    } else if (accumulated.size() > 32) {

                        accumulated.clear(); // If the buffer starts with something unrecognizable and grows too large

                    } else  {

                        Event e; e.type = EventType::KEY_PRESS;
                        e.keyboard.key = accumulated[0]; e.keyboard.modifiers = KeyModifiers::NONE;
                        out_event_queue.push(e);

                        accumulated.erase(0, 1);

                    } 

                }

            }
        } else if (ret == 0) {
            // Timeout reached, no input detected
        } else {
            perror("poll"); // An error occurred
            break;
        }

    }
    
}