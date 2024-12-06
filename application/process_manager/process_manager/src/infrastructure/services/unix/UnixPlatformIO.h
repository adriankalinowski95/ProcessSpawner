#pragma once

#include <stdexcept>
#include <process_manager/src/application/services/IPlatformIO.h>

namespace process_manager::infrastructure::services::unix {

class UnixPlatformIO : public process_manager::application::services::IPlatformIO {
public:
    bool writeData(int handle, std::string_view data) override {
        ssize_t result = write(handle, data.data(), data.size());

        return (result == static_cast<ssize_t>(data.size()));
    }

    std::optional<std::string> readDataNonBlocking(int handle) override {
        // Upewniamy się, że deskryptor jest nieblokujący (w razie potrzeby można użyć fcntl)
        // Dla uproszczenia załóżmy, że jest już nieblokujący lub po prostu czytamy dane dostępne w tym momencie.
        char buffer[256];
        ssize_t bytesRead = read(handle, buffer, sizeof(buffer));
        if (bytesRead > 0) {
            return std::string(buffer, bytesRead);
        } else if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            // Brak danych w tej chwili
            return std::nullopt;
        }

        // EOF lub błąd
        return std::nullopt;
    }

    void closeHandle(int handle) override {
        if (handle <= 0) {
            throw std::runtime_error("Invalid handle");
        }

        close(handle);
    }
};

}