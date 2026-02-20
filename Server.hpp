#include <sys/socket.h>
#include <netinet/in.h> // pour5 le domaine (AF_INET cest a direr IPv4)
#include <sys/types.h> // le type ici sock_stream pour recevoir les messages dans lordre et ne pas perdre une partie du message
IPPROTO_TCP  // pour TCP (👉 Les messages envoyés entre le client et le serveur, arrivent toujours, arrivent dans le bon ordre, ne sont pas corrompus
