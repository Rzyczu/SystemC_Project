#include <systemc.h>
#include <queue>
#include <string>

// ErrorProcessor module for handling error messages via FIFO
SC_MODULE(ErrorProcessor) {
    sc_in<bool> clk;          // Sygna� zegara
    sc_in<bool> reset;        // Sygna� resetu

    sc_out<int> data_out;     // Dane wyj�ciowe
    sc_in<int> data_in;       // Dane wej�ciowe
    sc_out<int> address_out;  // Adres wyj�ciowy

    // FIFO queue for error messages
    std::queue<int> error_fifo;

    // Error processing state
    bool processing_error;

    SC_CTOR(ErrorProcessor) : processing_error(false) {
        SC_METHOD(process);
        sensitive << clk.pos(); // Wra�liwo�� na dodatni brzeg zegara

        // Event for notifying new errors
        SC_METHOD(check_fifo);
        sensitive << clk.pos();
    }

    void process() {
        if (reset.read()) {
            // Resetowanie procesora b��d�w
            data_out.write(0);
            address_out.write(0);

            // Clear FIFO queue
            while (!error_fifo.empty()) {
                error_fifo.pop();
            }

            processing_error = false;
        }
        else {
            // Sprawdzenie czy przyszed� nowy b��d
            int input_data = data_in.read();

            // Przyjmujemy, �e kod b��du 7 to sygna� do obs�ugi
            if (input_data != 0) {
                // Dodajemy b��d do kolejki FIFO
                error_fifo.push(input_data);

                // Wysy�amy potwierdzenie przyj�cia b��du
                data_out.write(0xE0); // Kod potwierdzenia b��du
                address_out.write(0xF0); // Specjalny adres dla b��d�w
            }
        }
    }

    void check_fifo() {
        if (!reset.read()) {
            // Je�li mamy b��dy w kolejce i nie przetwarzamy aktualnie
            if (!error_fifo.empty() && !processing_error) {
                // Pobieramy pierwszy b��d z kolejki
                int error_code = error_fifo.front();
                error_fifo.pop();

                // Rozpoczynamy przetwarzanie
                processing_error = true;

                // Symulujemy przetwarzanie b��du
                data_out.write(0xE1); // Kod przetwarzania b��du
                address_out.write(0xF1); // Adres informuj�cy o przetwarzaniu
            }
            else if (processing_error) {
                // Zako�czenie przetwarzania b��du
                processing_error = false;

                // Wysy�amy kod zako�czenia przetwarzania
                data_out.write(0xE2); // Kod zako�czenia obs�ugi b��du
                address_out.write(0xF2); // Adres informuj�cy o zako�czeniu
            }
        }
    }
};