#include <systemc.h>
#include <queue>
#include <string>

// ErrorProcessor module for handling error messages via FIFO
SC_MODULE(ErrorProcessor) {
    sc_in<bool> clk;          // Sygna³ zegara
    sc_in<bool> reset;        // Sygna³ resetu

    sc_out<int> data_out;     // Dane wyjœciowe
    sc_in<int> data_in;       // Dane wejœciowe
    sc_out<int> address_out;  // Adres wyjœciowy

    // FIFO queue for error messages
    std::queue<int> error_fifo;

    // Error processing state
    bool processing_error;

    SC_CTOR(ErrorProcessor) : processing_error(false) {
        SC_METHOD(process);
        sensitive << clk.pos(); // Wra¿liwoœæ na dodatni brzeg zegara

        // Event for notifying new errors
        SC_METHOD(check_fifo);
        sensitive << clk.pos();
    }

    void process() {
        if (reset.read()) {
            // Resetowanie procesora b³êdów
            data_out.write(0);
            address_out.write(0);

            // Clear FIFO queue
            while (!error_fifo.empty()) {
                error_fifo.pop();
            }

            processing_error = false;
        }
        else {
            // Sprawdzenie czy przyszed³ nowy b³¹d
            int input_data = data_in.read();

            // Przyjmujemy, ¿e kod b³êdu 7 to sygna³ do obs³ugi
            if (input_data != 0) {
                // Dodajemy b³¹d do kolejki FIFO
                error_fifo.push(input_data);

                // Wysy³amy potwierdzenie przyjêcia b³êdu
                data_out.write(0xE0); // Kod potwierdzenia b³êdu
                address_out.write(0xF0); // Specjalny adres dla b³êdów
            }
        }
    }

    void check_fifo() {
        if (!reset.read()) {
            // Jeœli mamy b³êdy w kolejce i nie przetwarzamy aktualnie
            if (!error_fifo.empty() && !processing_error) {
                // Pobieramy pierwszy b³¹d z kolejki
                int error_code = error_fifo.front();
                error_fifo.pop();

                // Rozpoczynamy przetwarzanie
                processing_error = true;

                // Symulujemy przetwarzanie b³êdu
                data_out.write(0xE1); // Kod przetwarzania b³êdu
                address_out.write(0xF1); // Adres informuj¹cy o przetwarzaniu
            }
            else if (processing_error) {
                // Zakoñczenie przetwarzania b³êdu
                processing_error = false;

                // Wysy³amy kod zakoñczenia przetwarzania
                data_out.write(0xE2); // Kod zakoñczenia obs³ugi b³êdu
                address_out.write(0xF2); // Adres informuj¹cy o zakoñczeniu
            }
        }
    }
};