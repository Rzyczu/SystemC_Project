#include <systemc.h>

SC_MODULE(NiosIIProcessor) {
    sc_in<bool> clk;          // Sygna� zegara
    sc_in<bool> reset;        // Sygna� resetu

    sc_out<int> data_out;     // Dane wyj�ciowe
    sc_in<int> data_in;       // Dane wej�ciowe
    sc_out<int> address_out;  // Adres wyj�ciowy (dodany port)

    int internal_register;    // Rejestr wewn�trzny procesora (np. akumulator)

    sc_event interrupt_event; // Zdarzenie przerwania

    SC_CTOR(NiosIIProcessor) : internal_register(0) {
        SC_METHOD(process);
        sensitive << clk.pos(); // Wra�liwo�� na dodatni brzeg zegara
    }

    void process() {
        if (reset.read()) {
            // Procesor w stanie resetu
            data_out.write(0);
            address_out.write(0);  // Ustawienie adresu na 0 w przypadku resetu
            internal_register = 0; // Resetowanie rejestru
        }
        else {
            // Je�li wyst�pi�o przerwanie, obs�ugujemy je
            if (interrupt_event.triggered()) {
                // Obs�uguje przerwanie
                internal_register = data_in.read();
                data_out.write(internal_register);
                address_out.write(0x1); // Przyk�adowy adres
                interrupt_event.cancel(); // Resetowanie przerwania po obs�udze
            }
            else {
                // Normalna logika procesora
                internal_register = data_in.read();
                data_out.write(internal_register);  // Wyj�cie z danych
                address_out.write(0x2); // Inny adres
            }
        }
    }

    void trigger_interrupt() {
        interrupt_event.notify(SC_ZERO_TIME);  // Generowanie przerwania
    }
};
