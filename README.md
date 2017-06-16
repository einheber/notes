# notes
a sample REST API for notes

To install on Linux:

1. ensure that you have g++ cmake and git installed
   # which g++ cmake git

2. clone this repo

   # git clone https://github.com/einheber/notes.git
   # cd notes

3. build (this will clone and build the pistache and rapidjson frameworks)

   # make

4. run the notes server (first ensure nothing is running on port 80)

   # LD_LIBRARY_PATH=./pistache/build/src ./nserver&

   Note: You'll need to run as root to bind port 80

