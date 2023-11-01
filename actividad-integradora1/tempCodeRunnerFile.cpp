vector<string>transmissions;
    vector<string>mcodes;
    // archivos de transmisión
    string transmission1 = leeArchivo("./transmission1.txt");
    string transmission2 = leeArchivo("./transmission2.txt");

    // archivos de código malicioso
    string mcode1 = leeArchivo("./mcode1.txt");
    string mcode2 = leeArchivo("./mcode2.txt");
    string mcode3 = leeArchivo("./mcode3.txt");

    transmissions.push_back(transmission1);
    transmissions.push_back(transmission2);

    mcodes.push_back(mcode1);
    mcodes.push_back(mcode2);
    mcodes.push_back(mcode3);

    muestraResultado(transmissions, mcodes);