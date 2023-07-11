 
 
 
 char* pBuffer;                              // Declare a pointer to your buffer.
    myFile = SD.open(F("fileName.txt"));        // Open file for reading.
    if (myFile){
        unsigned int fileSize = myFile.size();  // Get the file size.
        pBuffer = (char*)malloc(fileSize + 1);  // Allocate memory for the file and a terminating null char.
        myFile.read(pBuffer, fileSize);         // Read the file into the buffer.
        pBuffer[fileSize] = '\0';               // Add the terminating null char.
        Serial.println(pBuffer);                // Print the file to the serial monitor.
        myFile.close();                         // Close the file.
    }
    // *** Use the buffer as needed here. ***
    free(pBuffer);                              // Free the memory that was used by the buffer.