
void AI_init();
void DAC_init();

int readChannel(uint8_t channel);
void DAC_output(int value);
void DAC_outputSineWave(double amplitude, double frequency);
