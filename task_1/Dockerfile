# Указываем базовый образ с поддержкой OpenMPI
FROM ubuntu:22.04

# Устанавливаем необходимые инструменты и зависимости
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    libopenmpi-dev

# Указываем рабочую директорию внутри контейнера
WORKDIR /app

# Копируем исходный код программы в контейнер
COPY main.cpp .

# Компилируем программу с использованием MPI компилятора
RUN mpic++ -o mpi_program main.cpp

# Указываем команду запуска (по умолчанию запускаем с двумя процессами)
CMD ["mpirun", "-np", "2", "./mpi_program"]
