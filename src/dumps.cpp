
/*
    void dump(const void * mem, int count)
    {
      for (int i = 0; i < count; i++)
      {
        char buff[80];
        if (i % 8 == 0)
        {
          sprintf(buff, "\n%08p: ", (void *) mem);
          Serial.print(buff);
        }
      int byte = pgm_read_byte(mem);
      sprintf(buff, " %03d", byte);

      Serial.print(buff);
      mem++;  
      }
      Serial.println();
    }

    void dumpHex(const void * mem, int count)
    {
      for (int i = 0; i < count; i++)
      {
        char buff[80];
        if (i % 8 == 0)
        {
          sprintf(buff, "\n%08p: ", (void *) mem);
          Serial.print(buff);
        }
      int byte = pgm_read_byte(mem);
      sprintf(buff, " %02x", byte);

      Serial.print(buff);
      mem++;  
      }
      Serial.println();
    }

    */