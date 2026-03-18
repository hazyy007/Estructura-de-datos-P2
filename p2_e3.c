 
 #include "radio.h"
 #include "stack.h"
 #include "music.h"
 
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 

 int main(int argc, char **argv)
 {

    Radio *r1 = NULL;
    FILE *f = NULL;
    long id_orig = -1, id_dest = -1;
    if(argc != 4){
        fprintf(stderr, "Error al ejecutar los archivos");
        return EXIT_FAILURE;
    }
    f = fopen(argv[1], "r");
    if(!f)
    {
        fprintf(stderr, "Error al abrir el archivo");
        return EXIT_FAILURE;
    }
    id_orig = atol(argv[2]);
    id_dest = atol(argv[3]);

    r1 = radio_init();
    if(!r1){
        fprintf(stderr, "Error al crear la radio");
        return EXIT_FAILURE;
    }
    
    if(!radio_readFromFile(f, r1))
    {
        fprintf(stderr, "Error al leer el archivo");
        radio_free(r1);
        fclose(f);
        return EXIT_FAILURE;
    }

    radio_print(stdout, r1);

   if(!radio_depthSearch(r1, id_orig, id_dest))
   {
    fprintf(stderr, "Error al realizar la búsqueda en profundidad");
   }

   radio_free(r1);
   return EXIT_SUCCESS;
}
 