#include "battery_reader.h" // Includi il tuo file header

// Costruttore
battery_reader::battery_reader(int update_interval_seconds) 
    : update_interval_seconds_(update_interval_seconds), 
      current_status_("N/A"), // Valore di default iniziale
      last_read_time_(std::chrono::steady_clock::now()) 
{
    // Tentiamo una lettura iniziale
    read_file_if_needed();
}

// Implementazione della logica di lettura con controllo del tempo
void battery_reader::read_file_if_needed() {
    
    // 1. Controllo del Tempo: 
    // Se non è passato abbastanza tempo dall'ultima lettura, usciamo
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - last_read_time_).count() < update_interval_seconds_) {
        return;
    }
    
    // 2. Lettura del File:
    std::ifstream file(BATTERY_FILE_PATH);
    std::string line;
    
    if (file.is_open()) {
        // Legge la prima riga del file
        std::getline(file, line);
        file.close();
        
        // 3. Aggiornamento dello Stato:
        // Blocca il mutex per aggiornare la variabile condivisa
        std::lock_guard<std::mutex> lock(status_mutex_);
        if (!line.empty()) {
            current_status_ = line;
        } else {
            current_status_ = "N/A"; // Il file è vuoto
        }
        
        // Aggiorna il tempo dell'ultima lettura riuscita
        last_read_time_ = now;
        
    } else {
        // Se il file non può essere aperto (es. non esiste ancora o problemi di permessi)
        std::lock_guard<std::mutex> lock(status_mutex_);
        current_status_ = "File Error";
        // Non aggiornare last_read_time_ qui, per riprovare subito alla prossima chiamata
    }
}

// Metodo pubblico per recuperare lo stato
std::string battery_reader::get_battery_status() {
    // Prima di restituire lo stato, tentiamo di aggiornarlo se è passato abbastanza tempo
    read_file_if_needed(); 
    
    // Blocca e restituisce l'ultima stringa letta
    std::lock_guard<std::mutex> lock(status_mutex_);
    return current_status_;
}