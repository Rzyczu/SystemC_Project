#include "Top.cpp"
#include <iostream>
#include <limits>

int sc_main(int argc, char* argv[]) {
    Top top("Top");

    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> reset_sig("reset_sig");

    sc_signal<int> processor_data_out_sig("processor_data_out_sig");
    sc_signal<int> processor_address_out_sig("processor_address_out_sig");
    sc_signal<int> processor_data_in_sig("processor_data_in_sig");

    sc_signal<int> memory_data_out_sig("memory_data_out_sig");
    sc_signal<int> memory_data_in_sig("memory_data_in_sig");

    sc_signal<int> switch_button_input_sig("switch_button_input_sig");
    sc_signal<int> switch_button_output_sig("switch_button_output_sig");

    sc_signal<int> error_processor_data_in_sig("error_processor_data_in_sig");

    sc_signal<sc_uint<3>> sw_input_sig("sw_input_sig");
    sc_signal<sc_uint<7>> hex_output_left_sig("hex_output_left_sig");
    sc_signal<sc_uint<7>> hex_output_right_sig("hex_output_right_sig");
    sc_signal<bool> alarm_led_sig("alarm_led_sig");
    sc_signal<sc_uint<7>> alarm_code_sig("alarm_code_sig");

    top.clk(clk);
    top.reset(reset_sig);

    top.processor_data_out(processor_data_out_sig);
    top.processor_address_out(processor_address_out_sig);
    top.processor_data_in(processor_data_in_sig);

    top.memory_data_out(memory_data_out_sig);
    top.memory_data_in(memory_data_in_sig);

    top.switch_button_input(switch_button_input_sig);
    top.switch_button_output(switch_button_output_sig);

    top.sw_input(sw_input_sig);
    top.hex_output_left(hex_output_left_sig);
    top.hex_output_right(hex_output_right_sig);
    top.alarm_led(alarm_led_sig);
    top.alarm_code(alarm_code_sig);

    top.error_processor_data_in = error_processor_data_in_sig;

    std::cout << "\n--- RESET ---" << std::endl;
    reset_sig.write(true);
    sc_start(10, SC_NS);
    reset_sig.write(false);

    std::cout << "--- START SIMULATION ---\n" << std::endl;

    bool running = true;
    while (running) {
        std::cout << "\nSelect a heat source:\n";
        std::cout << " 1. Burner 1 (P1)\n";
        std::cout << " 2. Burner 2 (P2)\n";
        std::cout << " 3. Burner 3 (P3)\n";
        std::cout << " 4. Burner 4 (P4)\n";
        std::cout << " 5. Oven (PP)\n";
        std::cout << " 0. Manual RESET\n";
        std::cout << " 6. Invalid choice (for error simulation)\n";
        std::cout << " 7. Exit\n";
        std::cout << "Your selection: ";

        int code;
        if (!(std::cin >> code)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (code == 7) {
            std::cout << "\nShutting down simulation...\n";
            running = false;
            break;
        }

        if (code == 0) {
            std::cout << "\n[RESET triggered]\n";
            reset_sig.write(true);
            sc_start(1, SC_NS);
            reset_sig.write(false);
            continue;
        }

        if (code >= 1 && code <= 5) {
            sw_input_sig.write(code);
        }
        else {
            sw_input_sig.write(0b111);
            top.error_processor_data_in.write(code);
            top.trigger_processor_interrupt();
        }

        sc_start(10, SC_NS);

        std::cout << "\nSelected: ";
        switch (code) {
        case 1: std::cout << "Burner 1 (P1)"; break;
        case 2: std::cout << "Burner 2 (P2)"; break;
        case 3: std::cout << "Burner 3 (P3)"; break;
        case 4: std::cout << "Burner 4 (P4)"; break;
        case 5: std::cout << "Oven (PP)"; break;
        case 0: std::cout << "[RESET]"; break;
        default: std::cout << "Invalid input (error triggered)"; break;
        }

        std::cout << "\nHEX Left : 0x" << std::hex << hex_output_left_sig.read().to_uint();
        std::cout << " | HEX Right: 0x" << hex_output_right_sig.read().to_uint();
        if (!alarm_led_sig.read()) {
            std::cout << " | Alarm LED ON: 0x" << alarm_code_sig.read().to_uint() << std::dec << "\n";
        }
        else {
            std::cout << " | Alarm LED: OFF\n";
        }
    }

    return 0;
}
