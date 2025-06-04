#include "Bus.cpp"
#include "NiosIIProcessor.cpp"
#include "ErrorProcessor.cpp"
#include "OnChipMemory.cpp"
#include "Switch.cpp"
#include "GasSelector.cpp"
#include <systemc.h>

SC_MODULE(Top) {
    sc_in<bool> clk;
    sc_in<bool> reset;

    sc_signal<int> bus_data;
    sc_signal<int> bus_address;
    sc_signal<bool> bus_read_write;
    sc_signal<bool> bus_chip_select;

    sc_in<int> processor_data_in;
    sc_out<int> processor_data_out;
    sc_out<int> processor_address_out;

    sc_signal<int> error_processor_data_in;
    sc_signal<int> error_processor_data_out;
    sc_signal<int> error_processor_address;

    sc_signal<int> memory_address;
    sc_in<int> memory_data_in;
    sc_out<int> memory_data_out;

    sc_in<int> switch_button_input;
    sc_out<int> switch_button_output;

    sc_out<sc_uint<3>> sw_input;
    sc_out<sc_uint<7>> hex_output_left;
    sc_out<sc_uint<7>> hex_output_right;
    sc_out<sc_uint<7>> alarm_code;
    sc_out<bool> alarm_led;

    Bus bus;
    NiosIIProcessor processor;
    ErrorProcessor error_proc;
    OnChipMemory memory;
    Switch button_switch;
    GasSelector gas_selector;

    SC_CTOR(Top)
        : bus("Bus"),
        processor("NiosIIProcessor"),
        error_proc("ErrorProcessor"),
        memory("OnChipMemory"),
        button_switch("Switch"),
        gas_selector("GasSelector"),
        bus_data("bus_data"),
        bus_address("bus_address"),
        bus_read_write("bus_read_write"),
        bus_chip_select("bus_chip_select"),
        error_processor_data_in("error_processor_data_in"),
        error_processor_data_out("error_processor_data_out"),
        error_processor_address("error_processor_address"),
        memory_address("memory_address")
    {
        bus.clk(clk);
        bus.reset(reset);
        bus.data(bus_data);
        bus.address(bus_address);
        bus.read_write(bus_read_write);
        bus.chip_select(bus_chip_select);

        processor.clk(clk);
        processor.reset(reset);
        processor.data_out(processor_data_out);
        processor.address_out(processor_address_out);
        processor.data_in(processor_data_in);

        error_proc.clk(clk);
        error_proc.reset(reset);
        error_proc.data_out(error_processor_data_out);
        error_proc.address_out(error_processor_address);
        error_proc.data_in(error_processor_data_in);

        memory.clk(clk);
        memory.reset(reset);
        memory.address(memory_address);
        memory.data_in(bus_data);
        memory.data_out(memory_data_out);

        button_switch.clk(clk);
        button_switch.reset(reset);
        button_switch.button_input(switch_button_input);
        button_switch.button_output(switch_button_output);

        gas_selector.clk(clk);
        gas_selector.reset(reset);
        gas_selector.sw_input(sw_input);
        gas_selector.hex_output_left(hex_output_left);
        gas_selector.hex_output_right(hex_output_right);
        gas_selector.alarm_led(alarm_led);
        gas_selector.alarm_code(alarm_code);
    }

    void trigger_processor_interrupt() {
        processor.trigger_interrupt();
    }
};
