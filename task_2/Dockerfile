FROM ubuntu:22.04

# Устанавливаем необходимые инструменты и зависимости
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    libopenmpi-dev \
    libpq-dev

# Указываем рабочую директорию
WORKDIR /app

# Копируем исходный код
COPY main.cpp .

# Компилируем программу
RUN mpic++ -o mpi_program main.cpp -I/usr/include/postgresql -L/usr/lib/x86_64-linux-gnu -lpq

# Запускаем программу
CMD ["mpirun", "-np", "2", "./mpi_program"]