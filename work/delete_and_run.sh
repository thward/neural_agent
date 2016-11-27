touch neural.c
make
rm logfile.txt
rm connections.txt
rm connections_out.txt
./neural
gvim -geometry 500x500 logfile.txt &
