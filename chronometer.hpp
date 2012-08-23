#ifndef CHRONOMETER_HPP
#define CHRONOMETER_HPP

#include <sys/time.h>
#include <iostream>


namespace Time {

  class Chronometer {

  private:

    timeval begin, end;
    
  public:

    Chronometer(){}

    inline void start()
    {
      gettimeofday(&begin, NULL);
    }


    inline void stop()
    {
      gettimeofday(&end, NULL);
    }

    inline float dureeCalcule() const
    {
      return ((float)(end.tv_sec - begin.tv_sec) + (float)(end.tv_usec -  begin.tv_usec) /1000000);
    }

  };


  std::ostream& operator<<(std::ostream& out, const Chronometer& chrono) {
    out << chrono.dureeCalcule() <<" sec"<<std::endl;
    return out;
  }

}

#endif
