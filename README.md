### Intructions for setting up employee-manager

### Clone repository:

    :::sh
    # Clone repo
    git clone https://github.com/foxxydev/employee-manager.git
    cd employee-manager

### Compiling:

    :::ssh
    mkdir build
    cd build
    qmake ..
    make

### To show all debugging messages type:

    :::sh
    export QT_LOGGING_RULES="*=true"

### Execute the binary:

    :::sh
    ./employee-manager
