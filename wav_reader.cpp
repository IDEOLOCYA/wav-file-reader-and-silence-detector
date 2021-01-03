#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
using namespace std;
/*
    Yararlanilan kaynaklar:
    Wave formatlari ve struct yapisinin olusturulmasinda     http://soundfile.sapp.org/doc/WaveFormat/
    Kod parçalarının anlaşılmasında tekrar tekrar donup en son hah en basta buldugummus dedigim
    https://github.com/tkaczenko/WavReader/blob/master/WavReader/WavReader.cpp

*/

// void WavHeaderReader(FILE *dosya);
//void ReadNextSecondSamples(FILE *dosya, FILE *fout);
//void SilentDeciderNextSecond(); //inherited from  ReadNextSecondSamples

typedef struct ses{
  /* RIFF Chunk Descriptor */
uint8_t         RIFF[4];        // RIFF Header Magic header
uint32_t        ChunkSize;      // RIFF Chunk Size
uint8_t         WAVE[4];        // WAVE Header
/* "fmt" sub-chunk */
uint8_t         fmt[4];         // FMT header
uint32_t        Subchunk1Size;  // Size of the fmt chunk
uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
uint32_t        bytesPerSec;    // bytes per second
uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
uint16_t        bitsPerSample;  // Number of bits per sample
/* "data" sub-chunk */
uint8_t         Subchunk2ID[4]; // "data"  string
uint32_t        Subchunk2Size;  // Sampled data length
}wav_dosyasi;





class WavHeaderReader
{
public:
  //open file and reading part
  wav_dosyasi veri;
  int sample_size =0;
  int samples_count=0;
  
  void readHeader(FILE *dosya)
  {

  fread(&veri,sizeof(veri),1,dosya);

  //Printing part
  cout<<"dosya okunuyor:"<<endl;
  cout<<"File type: "<<veri.RIFF<<endl;
  cout<<"File Size: "<<veri.ChunkSize<<endl;
  cout<<"Wav Marker: "<<veri.WAVE<<endl;
  cout<<"Format Name: "<<veri.fmt<<endl;
  cout<<"Format Length: "<<veri.Subchunk1Size<<endl;
  cout<<"Format Type: "<<veri.AudioFormat<<endl;
  cout<<"Number of Channels: "<<veri.NumOfChan<<endl;
  cout<<"Sample Rate: "<<veri.SamplesPerSec<<endl;
  cout<<"Sample Rate*Bits/Sample *Channels/ 8: "<<veri.bytesPerSec<<endl;
  cout<<"Bits per Sample * Channels /8,1: "<<veri.blockAlign<<endl;
  cout<<"Bits per Sample: "<<veri.bitsPerSample<<endl;
  cout<<"Data size: "<<veri.Subchunk2Size<<endl;
  cout<<"data: "<<veri.Subchunk2ID<<endl<<endl;
  printf("%u %u %u %u \n\n", veri.Subchunk2ID[0], veri.Subchunk2ID[1], veri.Subchunk2ID[2], veri.Subchunk2ID[3]);
}

void readData(FILE *dosya)
  {
    FILE *fout = fopen("test.dat", "w");
    //Calculation of Number of samples
     int sample_size = veri.bitsPerSample / 8;
     int samples_count = veri.Subchunk2Size * 8 / veri.bitsPerSample;
     cout<<"Samples count: "<< samples_count<<endl;
     short int *value=new short int[samples_count];
     memset(value,0,sizeof(short int) * samples_count);

     //data samples reading part
     for(int i=0;i< samples_count;i++)
     {
       fread(&value[i],sample_size,1,dosya);
     }

     //Writing part of samples to a file

     for (int i = 0; i < samples_count; i++)
     {
       fprintf(fout, "%d\n", value[i]);
     }
       fclose(fout);
  }

};

int main()
{
  try{
  //const char * ses="/home/the-machine/Desktop/muhammet.wav"; //26 saniye
  const char * ses="/home/the-machine/Desktop/ahmet.wav"; //40 saniye
  FILE *dosya=fopen(ses,"rb");
  WavHeaderReader oku;
  oku.readHeader(dosya);
  oku.readData(dosya);
  fclose(dosya);

  }

  catch(...)
  {
    cout<<"hata yakalandı"<<endl;
  }
  cout<<endl<<"dosya basariyla okundu"<<endl;
}
