#/bin/bash
echo -e "\033[31mTo setup your case, open \"case.json\"\033[0m";
read -n 1 -p "Run the default case? (y/N) " -r yn;
echo "";
if [ $yn = "y" ]; then
    if [ -d "results" ]; then
        echo "Removing previous case";
        rm -fvr results;
    fi;
    ./NBody case.json;
    echo -e "\033[31mSimulation done";
    echo -e "The results are in \"results\" in CSV form \033[0m";
    read -p "Compress the data in a zip? (y/n)" -r yn;
    if [ $yn == "y" ]; then
        zip -r results.zip results;
    fi;
else
    echo "When ready, execture the program with this command line:"
    echo "./NBody case.json";
fi;
