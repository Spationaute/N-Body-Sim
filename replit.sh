#/bin/bash
#cd res/interface/;
#bash compileKotlin.sh;
#cd ~/NBody;
#cd res;
#python webInterface.py;

function runNBody {
    if [ -d "results" ]; then
        echo "Removing previous case";
        rm -fvr results;
    fi;
    ./NBody case.json;
    echo -e "\033[31mSimulation done";
    echo -e "The results are in \"results\" in CSV form \033[0m";
    read -n 1 -p "Compress the data in a zip? (y/n)" -r yn;
    if [ $yn == "y" ]; then
        zip -r results.zip results;
        echo -e "\033[32mResults are in results.zip\033[0m";
    else
        echo -e "\033[32mResults are in the results directory\033[0m";
    fi;
    read -n 1 -p "Launch Animation?" -r yn;
    if [ $yn == "y" ]; then
        octave replitAnime.m;
    else
        echo -e "Done";
    fi;
}

echo -e "\033[31mTo setup your case, open \"case.json\"\033[0m";
read -n 1 -p "Run the default case? (y/N) " -r yn;
echo "";
if [ $yn = "y" ]; then
    runNBody;
else
    echo -e "An editor will be open. Press ctrl+o to save changes."
    echo -e "ctrl+x to quit. The model will be run after with the new values."
    read -n 1;
    nano case.json;
    runNBody;
fi;
