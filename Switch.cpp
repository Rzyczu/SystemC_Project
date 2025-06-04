#include <systemc.h>

SC_MODULE(Switch) {
    sc_in<bool> clk;
    sc_in<bool> reset;

    sc_in<int> button_input;  // Wejœcie przycisku
    sc_out<int> button_output; // Wyjœcie przycisku

    SC_CTOR(Switch) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }

    void process() {
        if (reset.read()) {
            button_output.write(0);  // Reset state
        }
        else {
            // Convert bool to int using .read() method
            button_output.write(button_input.read() ? 1 : 0);
        }
    }
};
