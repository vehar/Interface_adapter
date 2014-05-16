void write_page (unsigned int adr, unsigned char function);
void fill_temp_buffer (unsigned int data,unsigned int adr);
unsigned int read_program_memory (unsigned int adr,unsigned char cmd);
void write_lock_bits (unsigned char val);
void enableRWW(void);


