//Le timer
class Timer
{
    private:
    //Le temps quand le timer est lancé
    int startTicks;

    //Les "ticks" enregistré quand le Timer a été mit en pause
    int pausedTicks;

    //Le status du Timer
    bool paused;
    bool started;

    public:
    //Initialise les variables (le constructeur)
    Timer();

    //Les différentes actions du timer
    void start();
    void stop();
    void pause();
    void unpause();

    //recupére le nombre de ticks depuis que le timer a été lancé
    //ou récupére le nombre de ticks depuis que le timer a été mis en pause
    int get_ticks();

    //Fonctions de vérification du status du timer
    bool is_started();
    bool is_paused();
};