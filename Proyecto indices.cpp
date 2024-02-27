//NOTA: El programa hace uso de los archivos Registros.txt (de texto) y datos.dat (binario), ademnas de un archivo temporal temp.dat

//Una futura opcion a agregar seria que a partir de otro archivo se cargue el archivo datos.dat, como lo que se hizo en la tarea 11
//  en donde de Registros.txt se jalaban los datos hacia datos.dat, en este programa datos.dat manda todo a Registros.txt para 
//  hacerlo legible al humano

//Si el producto a eliminar es el ultimo de datos.dat, entonces se seguira mostrando en este archivo, aunque no en Registros.txt, posible mejora


//Cada vez que se abre el programa, los productos toman un nuevo indice si es que se agregaron o eliminaron elementos
#include<iostream>
#include<algorithm>
#include<limits>
#include<vector>
constexpr int max = 50;

struct productos{
	char nombre[max];
	int codigo,costo,existenciaM,existenciaA;
};

struct indice{
	int llave; //codigo
	int RPN; //num de Registro
};

std::vector<indice> arrIndices; //Aqui van cuando los cargue

long fileSize(FILE* f) {
   fseek(f, 0, SEEK_END);
   long res = ftell(f);
   rewind(f);
   return res;
}

bool predicado(indice a, indice b){
	return a.llave < b.llave;
}
/*

//Cargamos el archivo datos.dat y registros.txt. El archivo registros.txt tiene todos los productos en formato de texto, mientras
// datos.dat los tiene en modo binario para manejarlos como bloques de informacion
FILE* abrirData(){
	FILE* ptD = fopen("DATOS.DAT","a+b"); // Lo abro en modo append, y ademas el + para lectura y escritura; la b para prevenir conversiones ocultas con windows
	if(ptD == nullptr){
		std::cout<<"Error al crear el archivo DATOS.DAT\n";
		return nullptr;
	}
	return ptD;
}

FILE* abrirRegistros(){
	FILE* ptR = fopen("Registros.txt","a+b");	// Lo abro en modo append, y ademas el + para lectura y escritura; la b para prevenir conversiones ocultas con windows
	if(ptR == nullptr){
		std::cout<<"Error al abrir el archivo Registros.txt\n";
		return nullptr;
	}
	return ptR;
}
*/

bool llenarIndicesYRegistro(){
	FILE* ptD = fopen("DATOS.DAT","rb"); // Lo abro en modo lectura; la b para prevenir conversiones ocultas con windows
	if(ptD == nullptr){
		std::cout<<"Error al abrir el archivo DATOS.DAT\n";
		return false;
	}
	
	FILE* ptR = fopen("Registros.txt","wb");	// Lo abro en modo escritura; la b para prevenir conversiones ocultas con windows
	if(ptR == nullptr){
		std::cout<<"Error al abrir el archivo Registros.txt\n";
		return false;
	}
	
	long tam = fileSize(ptD)/sizeof(productos); //AGREGAR AL REPORTE!!!!!!!!!, No uso while(!feof(ptD) porque al final me lee dos veces el ultimo producto
	for(long i = 0; i<tam; i++){
		productos temp;
		//fseek(ptD,sizeof(productos)*i,SEEK_SET);
		fread(&temp,sizeof(productos),1,ptD);
		
		if(temp.codigo > 0){
			fprintf(ptR,"%s %d %d %d %d",temp.nombre,temp.codigo,temp.costo,temp.existenciaM,temp.existenciaA); //Actualizo Registros.txt con lo nuevo de datos.dat
			//fprintf(stdout,"%s %d %d %d %d",temp.nombre,temp.codigo,temp.costo,temp.existenciaM,temp.existenciaA);
			//En que momento imrimir un espacio en Registros.txt AGREGAR AL REPORE!!!!!!!!!!
			indice temp2 = {temp.codigo,i};
			arrIndices.push_back(temp2);
		}
		//printf("%d\n",i);
	}

	std::sort(arrIndices.begin(),arrIndices.end(),predicado);
	
	/*
	Ejemplo de opciones descartadas:
	long indice = fileSize(ptD)/(long)sizeof(productos); // Me deberia de devolver el indice del ultimo producto;
	//printf("%d",indice);
	
	for(int i = 0; i<indice; i++){
		arrIndices[i] = true; //Los indices ocupados van de 0 hasta indices -1
	}
	*/
	fclose(ptD);
	fclose(ptR);
	return true;
}

bool agregarProducto(){
	FILE* ptD = fopen("DATOS.DAT","a+b"); // Lo abro en modo agregar, y ademas el + para lectura y escritura; la b para prevenir conversiones ocultas con windows
	if(ptD == nullptr){
		std::cout<<"Error al abrir el archivo DATOS.DAT\n";
		return false;
	}
	
	productos temp;
	temp.nombre[0] = '\n'; // ESTA SOLUCION ES LA QUE ENONCTRE CON RESPECTO A LOS ESPACIOS DE EL INICIO!
	std::cout<<"\tDigite el nombre del producto: ";
	scanf(" %[^\n]",&temp.nombre[1]);	//AGREGAR ESTO AL REPORTE!!!, esta fue la solucion que encontre, 
	
	int codigo;
	std::cout<<"\tDigite el codigo del producto: ";
	scanf("%d",&temp.codigo);
	
	indice buscado = {temp.codigo,-1}; 
	if(std::binary_search(&arrIndices[0],&arrIndices[0]+arrIndices.size(),buscado,predicado)){
		std::cout<<"\n!!Ya existe un producto con ese codigo :O\n\n";
		return false;
	}
	
	int costo;
	std::cout<<"\tDigite el costo del producto: ";
	scanf("%d",&temp.costo);
	
	int existenciaM;
	std::cout<<"\tDigite la existencia Minima del producto: ";
	scanf("%d",&temp.existenciaM);
	
	int existenciaA;
	std::cout<<"\tDigite la existencia Actual del producto: ";
	scanf("%d",&temp.existenciaA);
	
	long indiceActual = fileSize(ptD)/(long)sizeof(productos); // Me deberia de devolver el indice del ultimo producto;

	// Verificar que el codigo no exista es tardado :{
	indice temp2 = {temp.codigo,indiceActual};
	arrIndices.push_back(temp2);
	std::sort(arrIndices.begin(),arrIndices.end(),predicado);
	//arrIndices[indice] = true;
	
	fwrite(&temp,sizeof(productos),1,ptD); // Solo este fwrite para escribir sobre Datos.dat, ya esta en modo append
	
	//char espacio = '\n';
	//fwrite(&espacio,sizeof(char),1,ptD);
	
	std::cout<<"\nProducto agregado con exito :D\n\n";

	fclose(ptD);
	return true;
}

bool consultarProducto(){
	FILE* ptD = fopen("DATOS.DAT","rb"); // Lo abro en modo lectura, y ademas el + para lectura y escritura; la b para prevenir conversiones ocultas con windows
	if(ptD == nullptr){
		std::cout<<"Error al abrir el archivo DATOS.DAT\n";
		return false;
	}
	
	int codigo;
	std::cout<<"\t\tDigite el codigo a consultar: ";
	std::cin>>codigo;
			
	std::vector<indice>::iterator p = std::lower_bound(arrIndices.begin(),arrIndices.end(),indice{codigo,-1},predicado);
	if(p->llave == codigo){
		int lugar = p->RPN;
					
		productos temp;
		fseek(ptD,sizeof(productos)*lugar,SEEK_SET);
		fread(&temp,sizeof(productos),1,ptD);
					
		printf("\n\nENCONTRADO CON EXITO: %s %d %d %d %d\n\n\n",temp.nombre, temp.codigo, temp.costo, temp.existenciaM, temp.existenciaA);
	} else{
		std::cout<<"NO EXISTE tal producto\n\n";
	}
	
	fclose(ptD);
	return true;
}

bool eliminarProducto(){
	FILE* ptD = fopen("DATOS.DAT","r+b"); // Lo abro en modo lectura; la b para prevenir conversiones ocultas con windows
	if(ptD == nullptr){
		std::cout<<"Error al abrir el archivo DATOS.DAT\n";
		return false;
	}
	
	int codigo;
	std::cout<<"\t\tDigite el codigo del producto a eliminar: ";
	std::cin>>codigo;
	
	std::vector<indice>::iterator p = std::lower_bound(arrIndices.begin(),arrIndices.end(),indice{codigo,-1},predicado);
	if(p->llave == codigo){
		int lugar = p->RPN;
					
		productos temp;
		fseek(ptD,sizeof(productos)*(lugar),SEEK_SET);
			fread(&temp,sizeof(productos),1,ptD);
			printf("%s %d\n",temp.nombre,temp.codigo);
			temp.codigo = -1; //Lo marco como -1, eliminado
			
		fseek(ptD,sizeof(productos)*(lugar),SEEK_SET);
			fwrite(&temp,sizeof(productos),1,ptD);
		
		
			fseek(ptD,sizeof(productos)*(lugar),SEEK_SET);
			fread(&temp,sizeof(productos),1,ptD);
			printf("%s %d\n",temp.nombre,temp.codigo);
		
			
		p->llave = INT_MAX;
		std::sort(arrIndices.begin(),arrIndices.end(),predicado);
		arrIndices.pop_back();
	
		printf("\n\nEliminado con exito :D\n\n\n");
	} else{
		std::cout<<"El codigo no coincide con ningun producto\n\n";
	}
	
	fclose(ptD);
	return true;
}

//Elimina los codigos -1 de datos.dat y los mismos de Registros.txt
bool actualizarRegistro(){ 
	FILE* ptD = fopen("DATOS.DAT","r+b"); // Lo abro en modo lectura y ademas el + para escritura; la b para prevenir conversiones ocultas con windows
	if(ptD == nullptr){
		std::cout<<"Error al abrir el archivo DATOS.DAT\n";
		return false;
	}
	
	FILE* ptT = fopen("temp.dat","w+b");
	if(ptT == nullptr){
		std::cout<<"Error al abrir el archivo temp.DAT\n";
		return false;
	}
	
	//OTRA IDEA ERA TENER DOS APUNTADORES, UNO de lectura que avanzara solo si no hay un -1 en el codigo 
	// y el escritura que avanza cada que escribe
	long tam = fileSize(ptD)/sizeof(productos); //AGREGAR AL REPORTE!!!!!!!!!, No uso while(!feof(ptD) porque al final me lee dos veces el ultimo
	for(long i = 0; i<tam; i++){
		productos temp;
		//fseek(ptD,sizeof(productos)*i,SEEK_SET);
		fread(&temp,sizeof(productos),1,ptD);
		
		if(temp.codigo > 0){
			fwrite(&temp,sizeof(productos),1,ptT);
		}
	}
	rewind(ptT);
	fclose(ptD);
	
	ptD = fopen("DATOS.DAT","wb"); // Lo abro en modo escritura; la b para prevenir conversiones ocultas con windows
	if(ptD == nullptr){
		std::cout<<"Error al abrir el archivo DATOS.DAT\n";
		return false;
	}
	
	tam = fileSize(ptT)/sizeof(productos);
	for(long i = 0; i<tam; i++){
		productos temp;
		fread(&temp,sizeof(productos),1,ptT);
		fwrite(&temp,sizeof(productos),1,ptD);
	}
	
		
	fclose(ptT);
	fclose(ptD);
	
	llenarIndicesYRegistro();

	return true;
}

void consultaInventario(){
	FILE* ptD = fopen("DATOS.DAT","r+b"); // Lo abro en modo lectura y ademas el + para escritura; la b para prevenir conversiones ocultas con windows
	if(ptD == nullptr){
		std::cout<<"Error al abrir el archivo DATOS.DAT\n";
		return ;
	}
	
	//OTRA IDEA ERA TENER DOS APUNTADORES, UNO de lectura que avanzara solo si no hay un -1 en el codigo 
	// y el escritura que avanza cada que escribe
	long tam = fileSize(ptD)/sizeof(productos); //AGREGAR AL REPORTE!!!!!!!!!, No uso while(!feof(ptD) porque al final me lee dos veces el ultimo
	for(long i = 0; i<tam; i++){
		productos temp;
		fread(&temp,sizeof(productos),1,ptD);
		
		if(temp.codigo > 0){
			printf("%s %d %d %d %d",temp.nombre, temp.codigo, temp.costo, temp.existenciaM, temp.existenciaA);
		}
	}
	
	printf("\n\n\n");
	
	
	fclose(ptD);
	
}

int main(){
	bool inicio = llenarIndicesYRegistro(); // Al abrir el programa, lo primero que hara sera cargar cada uno de los RRN de los productos con sus respectivos indices, 
					// para inmediatamente ordenarlos y poder trabajar con ellos
	
	if(inicio){
		system("Color 0a");
		for(;;){
			std::cout<<"\t\tMENU :D \n";
			std::cout<<"1 - Agregar producto \n"; // Agrega y ordena automaticamente la lista de estos para la opcion de consulta
			std::cout<<"2 - Consultar producto \n"; // Busca de acuerdo a la llave en medio externo, y el indice sirve para ordenar en el programa
			std::cout<<"3 - Eliminar producto \n"; // Borramos el producto, el indice no se recorre para ahorrar tiempo
			std::cout<<"4 - Actualizar registro \n"; //En esta opcion ahora si recorremos los indices, de tal manera que si se elimino un elemento de enmedio
														//  tengamos continuidad en estos
			std::cout<<"5 - Consultar el inventario \n\n";
			std::cout<<"6 - Limpiar pantalla \n";
			std::cout<<"7 - Salir del programa \n";
			std::cout<<"\tElige: ";
			
			int opc;
			std::cin>>opc;
			if(opc == 1){
				printf("\n\n\t\t----------Agregar Producto---------\n\n");
				agregarProducto();
			} else if(opc == 2){
				printf("\n\n\t\t----------Consultar Producto---------\n\n"); //Justamente fue la tarea 11
				consultarProducto();
			} else if(opc == 3){
				printf("\n\n\t\t----------Eliminar Producto---------\n\n"); 
				eliminarProducto();
			} else if(opc == 4){ //Con base en datos.dat se eliminara de resigstros.txt
				printf("\n\n\t\t----------Actualizado Registros.txt---------\n\n"); //Se eliminan ahora si de Registro.txt para futuras consultas y tambien de Datos.dat
				actualizarRegistro();
				std::cout<<"-------------Operacion completada cone exito :D-------------\n\n";

			} else if(opc == 5){
					printf("\n\n\t\t----------INVENTARIO---------\n\n");
					consultaInventario();
			} else if(opc == 6){
				system("cls");
			} else if(opc == 7){
				llenarIndicesYRegistro();
				//actualizarRegistro();
				std::cout<<"Hasta luego :D\n\n";
				break;
			} else{
				std::cout<<"Elige una opcion valida\n";
			}
			system("pause");
			std::cout<<"\n\n";
		}

	} else{
		std::cout<<"Error al cargar los archivos :O\n\n";
	}
}
