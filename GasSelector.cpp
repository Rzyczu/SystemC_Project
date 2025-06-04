#include <systemc.h>

// Modu³ wyboru Ÿród³a ciep³a: palniki + piekarnik
SC_MODULE(GasSelector) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_uint<3>> sw_input;

    sc_out<sc_uint<7>> hex_output_left;   // Lewy znak (np. 'P')
    sc_out<sc_uint<7>> hex_output_right;  // Prawy znak (np. '1'...'4', 'P')
    sc_out<bool> alarm_led;
    sc_out<sc_uint<7>> alarm_code;

    void select_source() {
        if (reset.read()) {
            hex_output_left.write(0x7F);   // wszystkie segmenty wy³¹cz lone
            hex_output_right.write(0x7F);
            alarm_led.write(false);
            alarm_code.write(0x00);
        }
        else {
            switch (sw_input.read()) {
            case 0b001: // P1
                hex_output_left.write(0x73); // 'P'
                hex_output_right.write(0x06); // '1'
                alarm_led.write(false);
                alarm_code.write(0x06);
                break;
            case 0b010: // P2
                hex_output_left.write(0x73); // 'P'
                hex_output_right.write(0x5B); // '2'
                alarm_led.write(false);
                alarm_code.write(0x5B);
                break;
            case 0b011: // P3
                hex_output_left.write(0x73); // 'P'
                hex_output_right.write(0x4F); // '3'
                alarm_led.write(false);
                alarm_code.write(0x4F);
                break;
            case 0b100: // P4
                hex_output_left.write(0x73); // 'P'
                hex_output_right.write(0x66); // '4'
                alarm_led.write(false);
                alarm_code.write(0x66);
                break;
            case 0b101: // PP
                hex_output_left.write(0x73); // 'P'
                hex_output_right.write(0x73); // 'P'
                alarm_led.write(false);
                alarm_code.write(0x73);
                break;
            default: // B³¹d
                hex_output_left.write(0x7F); // Wszystkie segmenty
                hex_output_right.write(0x7F);
                alarm_led.write(true);
                alarm_code.write(0x00);
                break;
            }
        }
    }

    SC_CTOR(GasSelector) {
        SC_METHOD(select_source);
        sensitive << clk.pos();
    }
};
