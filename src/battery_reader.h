#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>

// Definiamo il percorso del file di stato creato dallo script Python
// Cambia questo se il tuo script Python salva il file altrove.
const std::string BATTERY_FILE_PATH = "/tmp/ups_display.txt";

class battery_reader {
public:
    // Costruttore: accetta l'intervallo minimo tra le letture (in secondi)
    battery_reader(int update_interval_seconds);

    // Metodo principale per l'integrazione: restituisce l'ultima stringa letta
    std::string get_battery_status();

private:
    std::string current_status_;
    std::chrono::steady_clock::time_point last_read_time_;
    int update_interval_seconds_;
    
    // Mutex per garantire la sicurezza del thread se il metodo viene chiamato da thread diversi
    std::mutex status_mutex_; 

    // Metodo interno che esegue l'operazione di lettura del file
    void read_file_if_needed();
};