PROYECTO EN 3 modern\skybox\projects\vs-2015
DLL en 3 modern\skybox\projects\ConsoleApplication1\ConsoleApplication1

DIOXY DOCS EN 3 modern\skybox\docs

La idea de la práctica era crear una herramienta para montar y guardar escenas usando meshes, texturas e iluminación.
El front lee un XML llamado a la DLL y se crea una ventana gráfica con todas las opciones para tratar la mesh(alguna vez me ha aparecido la ventana cortada, pero recompilando se arregla)
La ventana tiene unos datos metidos por defecto para que se cree un objeto en el medio
Los objetos se guardan por nombre único, si repites el mismo nombre se sustituirá el objeto antiguo por el nuevo
La cámara esta en el 0,0,0 por lo que los objetos tendran que tener una Z negativa para verse por pantalla
El boton "Create Mesh" crea una mesh con los valores que se han indicado en la ventana gráfica
El boton "Load XML" lee el XML y crea una escena con los datos que se encuentran dentro
El boton de "Save Scene" reescribe el XML que hay por parametro.
El boton "Delete Scene" borra todos los objetos de la escena.


