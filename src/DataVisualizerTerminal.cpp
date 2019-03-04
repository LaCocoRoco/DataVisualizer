#include "DataVisualizerTerminal.h"

DataVisualizerTerminal::DataVisualizerTerminal(void) {
    this->id = 0;
    this->scan = 0;
    this->visualizer = 0;
    this->packet = 0;
}

uint16_t DataVisualizerTerminal::available(void) {
    if(!this->visualizer) return 0;
    if(!this->visualizer->handshake) return 0;

    return this->packet->sent;
}

char DataVisualizerTerminal::read(void) {
    if(!this->visualizer) return 0;
    if(!this->visualizer->handshake) return 0;

    if(this->packet->sent) {
        char value = packet->data[this->scan++];

        if(this->scan == this->packet->sent) {
            memset(this->packet->data, 0, this->packet->length);
            this->scan = 0;
            this->packet->sent = 0;
        }

        return value;
    }

    return 0;
}

void DataVisualizerTerminal::read(char* buffer) {
    if(!this->visualizer) return;
    if(!this->visualizer->handshake) return;

    if(this->packet->sent) {
        memcpy(buffer, this->packet->data, this->packet->sent);
        memset(this->packet->data, 0, this->packet->length);
        this->scan = 0;
        this->packet->sent = 0;
    }
}

void DataVisualizerTerminal::print(char c) {
    this->print(&c);
}

void DataVisualizerTerminal::print(int n) {
    this->print((long) n);
}

void DataVisualizerTerminal::print(long n) {
    char c[ITOA_MAX_LENGTH];
    itoa(n, c, ITOA_DEC);
    this->print(c);
}

void DataVisualizerTerminal::print(const char* c) {
    if(!this->visualizer) return;
    if(!this->visualizer->handshake) return;

    this->visualizer->write(this->id, (uint8_t*) c, strlen(c));
}

void DataVisualizerTerminal::println(char c) {
    this->println(&c);
}

void DataVisualizerTerminal::println(int n) {
    this->println((long) n);
}

void DataVisualizerTerminal::println(long n) {
    char c[ITOA_MAX_LENGTH];
    itoa(n, c, ITOA_DEC);
    this->println(c);
}

void DataVisualizerTerminal::println(const char *c) {
    if(!this->visualizer) return;
    if(!this->visualizer->handshake) return;

    uint16_t length = strlen(c) + 1;
    char data[length];
    strncpy(data, c, length - 1);
    data[length - 1] = '\n';

    this->visualizer->write(this->id, (uint8_t*) data, length);
}