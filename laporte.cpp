

socket() → créer la porte
int porte_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)

bind() → dire où est la porte

listen() → mettre la porte en mode “prêt à accueillir du monde”

accept() → laisser entrer un client

fcntl(O_NONBLOCK) → ne pas bloquer si la porte est vide