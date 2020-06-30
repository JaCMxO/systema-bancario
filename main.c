#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ENDL printf("\n")
#define SYS_CLS system("cls")

typedef unsigned int uint_t;
typedef unsigned long long int id_t;
typedef unsigned long long int ulli_t;
typedef enum{false, true, nem} bool;

#pragma region Tipos de datos estructurados
struct str_fecha{
    unsigned int d, m, a;
};
typedef struct str_fecha Fecha;

struct str_usuario{
    id_t id_usuario;
    char Nombre[20];
    char apellidoMaterno[20];
    char apellidoPaterno[20];
    Fecha fecha;
};
typedef struct str_usuario *Usuario;

struct str_cuenta{
    id_t id_cuenta;
    id_t id_usuario;
    double saldo;
    Fecha fechaApertura;
};
typedef struct str_cuenta *Cuenta;

struct str_transaccion{
    id_t id_transaccion;
    char *tipo_transaccion;
    id_t id_cuenta_origen;
    id_t id_cuenta_destino;
    Fecha fecha;
    double monto;
};
typedef struct str_transaccion *Transaccion;
#pragma endregion Tipos de datos estructurados

#pragma region Prototipos Funciones Generales
void initFile();
ulli_t getFileSize(const char*);
unsigned int mydbMenu();
Fecha getDate();
#pragma endregion Prototipos Funciones Generales

#pragma region Prototipos Clientes
Usuario getFileInfoUser(const char*);
void writeUsuarioNuevo(Usuario, const char*, ulli_t);
void writeTodo(Usuario, const char*, ulli_t);
void nuevoCliente(const char*);
void buscarCliente(const char*);
void eliminarCliente(const char*);
void imprimirUsuarios(const char*);
void menuClientes();
void eliminarCuentasDeClientes(const char *, id_t);
#pragma endregion Prototipos Clientes

#pragma region Prototipos Cuentas
void menuCuentas();
void escrituraBinarioCuenta(const char *, Cuenta);
void impresionBinarioCuenta(const char *);
void nuevaCuenta(const char *);
void eliminarCuenta(const char *);
void buscarCuenta(const char *);
Cuenta getFileInfoCuenta(const char *);
#pragma endregion Prototipos Cuentas

#pragma region Prototipos Transacciones
void menuTransacciones();
Transaccion getFileInfoTransaccion(const char *);
void writeAccFile(Cuenta, const char*, ulli_t);
void writeTranFile(Transaccion, const char*);
bool validateAccount(const char*, id_t, double);
void Deposito(const char*, const char*);
void Retiro(const char*, const char*);
void Transferencia(const char*, const char*);
#pragma endregion Prototipos Transacciones

int main()
{
    uint_t c;
    initFile();

    do{
        c = mydbMenu();
        if(c == 1){
            SYS_CLS;
            menuClientes();
        }
        if(c == 2){
            SYS_CLS;
            menuCuentas();
        }
        if(c == 3){
            SYS_CLS;
            menuTransacciones();
        }
    }while(c != 4);

    return 0;
}

#pragma region Funciones Generales
void initFile()
{
    FILE *mydb = fopen("mydb.sys", "rb");
    if (mydb == NULL)
    {
        char direccion[256];
        printf("Introduce la direccion a donde deseas guardar los archivos de configuracion: \nPath: ");
        gets(direccion);

        mydb = fopen("mydb.sys", "wb");
        fputs(direccion, mydb);
        fclose(mydb);

        mydb = fopen("mydb.sys", "rb");

        FILE *clientes = fopen(strcat(fgets(direccion, 256, mydb), "\\clientes.dat"), "w");
        rewind(mydb);
        fclose(clientes);

        FILE *cuentas = fopen(strcat(fgets(direccion, 256, mydb), "\\cuentas.dat"), "w");
        rewind(mydb);
        fclose(cuentas);

        FILE *transacciones = fopen(strcat(fgets(direccion, 256, mydb), "\\transacciones.dat"), "w");
        fclose(transacciones);

        fclose(mydb);
    }
}

unsigned int mydbMenu(){
    unsigned int c;
    SYS_CLS;
    printf("<< Sistema MyDB >>\n");
    printf("1. Clientes\n");
    printf("2. Cuentas\n");
    printf("3. Transacciones\n");
    printf("4. Salir\n");
    printf("\nOpcion: ");
    scanf("%u", &c);
    ENDL;
    return c;
}

ulli_t getFileSize(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL)
    {
        printf("Error lectura getFileSize.\n\n");
    }
    fseek(f, 0, SEEK_END);
    ulli_t size = ftell(f);
    fclose(f);

    if (size == -1)
    {
        printf("Error al conseguir tama%co del archivo (getFileSize).\n\n", 164);
        return size;
    }
    return size;
}

Fecha getDate(){
    Fecha date;
    time_t t = time(NULL);
    struct tm tiempo = *localtime(&t);
    date.a = tiempo.tm_year + 1900;
    date.m = tiempo.tm_mon + 1;
    date.d = tiempo.tm_mday;
    return date;
}
#pragma endregion Funciones Generales

#pragma region Clientes
Usuario getFileInfoUser(const char* path){		//convertir el archivo a un arreglo
    ulli_t nElements = getFileSize(path) / (ulli_t)sizeof(struct str_usuario);
    Usuario newArr = (Usuario)malloc(nElements * sizeof(struct str_usuario));
    FILE *file = fopen(path, "rb");
    if(file){
        fread(newArr, sizeof(struct str_usuario), nElements, file);
    }else printf("Error al abrir el archivo...\n\n");
    fclose(file);
    return newArr;
}

void writeTodo(Usuario usuarios, const char* path, ulli_t nElements){
	Usuario newArr = (Usuario)malloc(nElements * sizeof(struct str_usuario));
	for(int i=0; i<nElements; i++){
		if(usuarios[i].id_usuario != -1){
			newArr[i] = usuarios[i];
		}
	}
	FILE *f = fopen(path, "wb");
	fwrite(newArr, sizeof(struct str_usuario), nElements, f);
	fclose(f);
	free(newArr);
}

void writeUsuarioNuevo(Usuario usuarios, const char* path, ulli_t nElements){
    FILE *f = fopen(path, "ab");
    fwrite(usuarios, sizeof(struct str_usuario), 1, f);
    fclose(f);
}

void menuClientes()
{
    FILE *direccion = fopen("mydb.sys", "r");
    char pathClientes[256];
    fgets(pathClientes, 256, direccion);
    fclose(direccion);
    strcat(pathClientes, "\\clientes.dat");
    char pathUsuarios[255];
    char opcion;
    do
    {
        do
        {
            printf(".:Clientes:.\n");
            printf("1. Cliente Nuevo.\n");
            printf("2. Buscar Cliente.\n");
            printf("3. Eliminar Cliente.\n");
            printf("4. Imprimir Clientes.\n");
            printf("5. Regresar.\n");
            printf("\nOpcion: ");
            scanf("%d", &opcion);
            ENDL;
        } while (opcion <= 0 || opcion >= 6);

        switch (opcion)
        {
        case 1:
            nuevoCliente(pathClientes);
            break;
        case 2:
            buscarCliente(pathClientes);
            break;
        case 3:
            eliminarCliente(pathClientes);
            break;
        case 4:
            imprimirUsuarios(pathClientes);
            break;
        case 5:
            break;
        }
    } while (opcion != 5);
}

void nuevoCliente(const char* PATH){
    ulli_t nElements = getFileSize(PATH) / (ulli_t)sizeof(struct str_usuario);
	Usuario newUsuario = (Usuario)malloc(sizeof(struct str_usuario));
	Usuario newUserArr = getFileInfoUser(PATH);
    Fecha hoy = getDate();

	setbuf(stdin,0);
	printf("Nombre: ");
	gets(newUsuario->Nombre);
	printf("Apellido Paterno: ");
	gets(newUsuario->apellidoPaterno);
	printf("Apellido Materno: ");
	gets(newUsuario->apellidoMaterno);

    if(nElements == 0){
        newUsuario->id_usuario = 1;
    }else{
        newUsuario->id_usuario = newUserArr[nElements - 1].id_usuario + 1;
    }

    do{
        printf("Dia de Nacimiento: ");
	    scanf("%u",&newUsuario->fecha.d);
        if(newUsuario->fecha.d < 1 && newUsuario->fecha.d > 31){
            printf("No has introducido un dia correcto\n");
        }
    }while(newUsuario->fecha.d < 1 && newUsuario->fecha.d > 31);

    do{
        printf("Mes de Nacimiento: ");
	    scanf("%u",&newUsuario->fecha.m);
        if(newUsuario->fecha.m < 1 && newUsuario->fecha.m > 12){
            printf("No has introducido un mes correcto\n");
        }
    }while(newUsuario->fecha.m < 1 && newUsuario->fecha.m > 12);
	
    do{
        printf("A%co de Nacimiento: ", 164);
	    scanf("%u",&newUsuario->fecha.a);
        if(newUsuario->fecha.a < (hoy.a - 150) && newUsuario->fecha.a > hoy.a){
            printf("No has introducido un a%co correcto.\n\n", 164);
        }
    }while(newUsuario->fecha.a < (hoy.a - 150) && newUsuario->fecha.a > hoy.a);
    ENDL;
	writeUsuarioNuevo(newUsuario, PATH, nElements);
	free(newUsuario);
	free(newUserArr);
}

void buscarCliente(const char* PATH){
	ulli_t nElements = getFileSize(PATH) / (ulli_t)sizeof(struct str_usuario);
	int c;
    bool userExists = false;
    ulli_t i;
	printf("ID a buscar: ");
	scanf("%d",&c);
	FILE *f = fopen(PATH, "rb");
	Usuario newArr = (Usuario)malloc(nElements * sizeof(struct str_usuario));
	fread(newArr, sizeof(struct str_usuario), nElements, f);
	for(i=0;i<nElements;i++){
		if(newArr[i].id_usuario == c){
            userExists = true;
            break;
		}
    }
    if(!userExists){
        printf("El cliente no existe.\n\n");
        ENDL;
    }else{
        printf("ID\tNombre\t\tApellido Paterno\tApellido Materno\tNacimiento\n");
        printf("%llu\t",newArr[i].id_usuario);
        printf("%-10s\t",newArr[i].Nombre);
        printf("%-16s\t",newArr[i].apellidoPaterno);
        printf("%-16s\t",newArr[i].apellidoMaterno);
        printf("%u/%u/%u\n",newArr[i].fecha.d,newArr[i].fecha.m,newArr[i].fecha.a);
        ENDL;
    }
	fclose(f);
	free(newArr);
}

void eliminarCliente(const char* PATH){
	ulli_t nElements = getFileSize(PATH) / (ulli_t)sizeof(struct str_usuario);
    id_t c;
    bool userExists = false;
    ulli_t i;
    printf("ID a Eliminar: ");
    scanf("%llu",&c);
    ENDL;
    Usuario newArr = getFileInfoUser(PATH);
    for(i=0;i<nElements;i++){
        if(newArr[i].id_usuario == c){
            userExists = true;
            break;
        }
    }
    if(!userExists){
        printf("El cliente no existe.\n\n");
    }else{
        FILE *archivo = fopen(PATH, "wb");
        fclose(archivo);
        newArr[i].id_usuario = 0;
    }
    for(ulli_t i = 0 ; i < nElements ; i++){
        if(newArr[i].id_usuario >= 1)
            writeUsuarioNuevo(&newArr[i], PATH, nElements);
    }
    free(newArr);
    ENDL;
    eliminarCuentasDeClientes(PATH, c);
    printf("Cliente eliminado exitosamente.\n\n");
    ENDL;
}

void imprimirUsuarios(const char* path){
	 ulli_t nElements = getFileSize(path) / (ulli_t)sizeof(struct str_usuario);
	 FILE *f = fopen(path, "rb");
	 Usuario newArr = getFileInfoUser(path);
	 ulli_t i=0;
	    if(f){
            fread(newArr, sizeof(struct str_usuario), nElements, f);
            printf("ID\tNombre\t\tApellido Paterno\tApellido Materno\tNacimiento\n");
            for(i=0;i<nElements;i++){
                if(newArr[i].id_usuario){
                    printf("%llu\t",newArr[i].id_usuario);
                    printf("%-10s\t",newArr[i].Nombre);
                    printf("%-16s\t",newArr[i].apellidoPaterno);
                    printf("%-16s\t",newArr[i].apellidoMaterno);
                    printf("%u/%u/%u\n",newArr[i].fecha.d,newArr[i].fecha.m,newArr[i].fecha.a);
                }
            }
            ENDL;
	    }else printf("Error al abrir el archivo...\n\n");
	fclose(f);
	free(newArr);
}

void eliminarCuentasDeClientes(const char *path, id_t id_cliente)
{
    // Si existe, elimina la cuenta deseada del sistema
    Cuenta complete = getFileInfoCuenta(path);
    ulli_t lenCuentas = getFileSize(path) / sizeof(struct str_cuenta);

    ENDL;
    for (ulli_t i = 0; i < lenCuentas; i++)
    {
        if (complete[i].id_cuenta == id_cliente)
        {
            complete[i].id_cuenta = 0;

            //Limpieza del archivo
            FILE *archivo = fopen(path, "wb");
            fclose(archivo);

            for (ulli_t j = 0; j < lenCuentas; j++)
            {
                if (complete[j].id_cuenta != 0)
                {
                    escrituraBinarioCuenta(path, &complete[j]);
                }
            }
            printf("\nCuentas del cliente eliminadas con exito.\n\n");
        }
    }
    free(complete);
    ENDL;
}
#pragma endregion Clientes

#pragma region Cuentas
void menuCuentas()
{
    FILE *direccion = fopen("mydb.sys", "r");
    char pathCuentas[256];
    fgets(pathCuentas, 256, direccion);
    fclose(direccion);
    strcat(pathCuentas, "\\cuentas.dat");
    char pathUsuarios[255];
    char opcion;
    do
    {
        do
        {
            printf(".:Cuentas:.\n");
            printf("1. Nueva cuenta.\n");
            printf("2. Buscar cuenta.\n");
            printf("3. Eliminar cuenta.\n");
            printf("4. Imprimir cuentas.\n");
            printf("5. Regresar.\n");
            printf("\nOpcion: ");
            scanf("%d", &opcion);
            ENDL;
        } while (opcion <= 0 || opcion >= 6);

        switch (opcion)
        {
        case 1:
            nuevaCuenta(pathCuentas);
            break;
        case 2:
            buscarCuenta(pathCuentas);
            break;
        case 3:
            eliminarCuenta(pathCuentas);
            break;
        case 4:
            impresionBinarioCuenta(pathCuentas);
            break;
        case 5:
            break;
        }
    } while (opcion != 5);
}

void escrituraBinarioCuenta(const char *path, Cuenta arr)
{
    FILE *archivo = fopen(path, "ab");
    if (archivo == NULL)
    {
        printf("\nNo se pudo abrir\n");
    }
    else
    {
        fwrite(arr, sizeof(struct str_cuenta), 1, archivo);
        fclose(archivo);
    }
}

void impresionBinarioCuenta(const char *path)
{
    // Imprime todas las cuentas registradas
    // id_cuenta, nombre de cliente, saldo de la cuenta.
    FILE *direccion = fopen("mydb.sys", "r");
    char pathClientes[256];
    fgets(pathClientes, 256, direccion);
    fclose(direccion);
    strcat(pathClientes, "\\clientes.dat");

    Cuenta arrC = getFileInfoCuenta(path);
    ulli_t size = getFileSize(path) / sizeof(struct str_cuenta);

    Usuario arrU = getFileInfoUser(pathClientes);
    ulli_t lenUsuario = getFileSize(pathClientes) / sizeof(struct str_usuario);

    printf("\nID\tNombre de cliente\t\tSaldo de la cuenta\tFecha de Apertura\n");
    for (ulli_t i = 0; i < size; i++)
    {
        for (ulli_t j = 0; j < lenUsuario; j++)
        {
            if (arrC[i].id_usuario == arrU[j].id_usuario)
            {
                printf("%llu\t%-10s%-10s,%-10s\t%-18.2lf", arrC[i].id_cuenta, arrU[j].apellidoPaterno, arrU[j].apellidoMaterno, arrU[j].Nombre, arrC[i].saldo);
                printf("\t%u/%u/%u\n", arrC[i].fechaApertura.d, arrC[i].fechaApertura.m, arrC[i].fechaApertura.a);
            }
        }
    }
    ENDL;
}

void nuevaCuenta(const char *pathCuentas)
{
    /* Registra una cuenta nueva a nombre de un usuario, utilice 
    id_cliente para relacionar el usuario y la cuenta. Antes de 
    crear la nueva cuenta se debe verificar que el usuario exista en el 
    sistema. Adicionalmente, se debe indicar el saldo con el que se 
    abre la cuenta. Por ejemplo; $1000. */
    Cuenta nvo = (Cuenta)calloc(1, sizeof(struct str_cuenta));
    FILE *direccion = fopen("mydb.sys", "r");
    char pathClientes[256];
    fgets(pathClientes, 256, direccion);
    fclose(direccion);
    strcat(pathClientes, "\\clientes.dat");
    Cuenta complete = getFileInfoCuenta(pathCuentas);
    ulli_t lenCuentas = getFileSize(pathCuentas) / sizeof(struct str_cuenta);
    Usuario usuario = getFileInfoUser(pathClientes);
    ulli_t lenUsuario = getFileSize(pathClientes) / sizeof(struct str_usuario);
    id_t id;
    printf("Introduce el id del cliente a relacionar con la cuenta: ");
    scanf("%llu", &id);
    ENDL;
    for (ulli_t i = 0; i < lenUsuario; i++)
    {
        if (usuario[i].id_usuario == id)
        {
            nvo->id_usuario = usuario[i].id_usuario;
            break;
        }
    }
    if (nvo->id_usuario)
    {
        if (lenCuentas == 0)
            nvo->id_cuenta = 1;
        else
            nvo->id_cuenta = complete[lenCuentas - 1].id_cuenta + 1;

        printf("Introduce el saldo inicial de la cuenta: $");
        scanf("%lf", &nvo->saldo);

        nvo->fechaApertura = getDate();

        escrituraBinarioCuenta(pathCuentas, nvo);

        printf("\nCuenta creada.\n");
    }
    else
    {
        printf("Id de cuenta no encontrado.\n");
    }
    ENDL;
    // Liberacion de memoria
    free(nvo);
    free(complete);
    free(usuario);
}

void buscarCuenta(const char *path)
{
    // Buscar usuario a partir de su id_cuenta en cuentas.dat
    // Imprimir id_cuenta, nombre de cliente, saldo de la cuenta
    FILE *direccion = fopen("mydb.sys", "r");
    char pathClientes[256];
    fgets(pathClientes, 256, direccion);
    fclose(direccion);
    strcat(pathClientes, "\\clientes.dat");

    int msgNotFound = 1;
    ulli_t id;
    printf("Introduce el ID de la cuenta a buscar: ");
    scanf("%llu", &id);
    ENDL;

    Cuenta arr = getFileInfoCuenta(path);
    ulli_t size = getFileSize(path) / sizeof(struct str_cuenta);
    Usuario arr2 = getFileInfoUser(pathClientes);
    ulli_t size2 = getFileSize(pathClientes) / sizeof(struct str_usuario);

    if (size != -1)     
    {
        printf("\nID\tNombre de cliente\t\tSaldo de la cuenta\tFecha de Apertura\n");
        for (ulli_t i = 0; i < size; i++)
        {
            if (arr[i].id_cuenta == id)
            {
                for (ulli_t j = 0; j < size2; j++)
                {
                    if (arr[i].id_usuario == arr2[j].id_usuario)
                    {
                        printf("%llu\t%-10s", arr[i].id_cuenta, arr2[j].apellidoPaterno);
                        printf("%-10s,%-10s\t%-18.2lf", arr2[j].apellidoMaterno, arr2[j].Nombre, arr[i].saldo);
                        printf("\t%u/%u/%u\n", arr[i].fechaApertura.d, arr[i].fechaApertura.m, arr[i].fechaApertura.a);
                    }
                }
                msgNotFound = 0;
                ENDL;
                break;
            }
        }
        if (msgNotFound)
            printf("Error 404: No se encontro ningun usuario con este id de cuenta.\n\n");
    }
    else
        printf("Se produjo un error a la hora de conseguir el tama%co de los archivos.\n\n", 164);
    ENDL;
    free(arr);
    free(arr2);
}

void eliminarCuenta(const char *path)
{
    // Si existe, elimina la cuenta deseada del sistema
    Cuenta complete = getFileInfoCuenta(path);
    ulli_t lenCuentas = getFileSize(path) / sizeof(struct str_cuenta);

    ulli_t aEliminar;
    printf("Introduce el id de la cuenta que deseas eliminar: ");
    scanf("%llu", &aEliminar);
    ENDL;
    for (ulli_t i = 0; i < lenCuentas; i++)
    {
        if (complete[i].id_cuenta == aEliminar)
        {
            complete[i].id_cuenta = 0;

            //Limpieza del archivo
            FILE *archivo = fopen(path, "wb");
            fclose(archivo);

            for (ulli_t j = 0; j < lenCuentas; j++)
            {
                if (complete[j].id_cuenta != 0)
                {
                    escrituraBinarioCuenta(path, &complete[j]);
                }
            }
            printf("\nCuenta eliminada con exito.\n\n");
            break;
        }
    }
    free(complete);
    ENDL;
}

Cuenta getFileInfoCuenta(const char *path)
{
    ulli_t nElements = getFileSize(path) / (ulli_t)sizeof(struct str_cuenta);
    Cuenta newArr = (Cuenta)malloc(nElements * sizeof(struct str_cuenta));
    FILE *file = fopen(path, "rb");
    if (file)
    {
        fread(newArr, sizeof(struct str_cuenta), nElements, file);
    }
    else
        printf("Error al abrir el archivo...\n");
    fclose(file);
    return newArr;
}
#pragma endregion Cuentas

#pragma region Transacciones
void menuTransacciones()
{
    FILE *direccion = fopen("mydb.sys", "r");
    char pathTransacciones[256];
    fgets(pathTransacciones, 256, direccion);
    fclose(direccion);

    FILE *f = fopen("mydb.sys", "r");
    char pathCuentas[256];
    fgets(pathCuentas, 256, f);
    fclose(f);

    strcat(pathCuentas, "\\cuentas.dat");
    strcat(pathTransacciones, "\\transacciones.dat");
    char pathUsuarios[255];
    char opcion;
    do
    {
        do
        {
            printf(".:Transacciones:.\n");
            printf("1. Deposito.\n");
            printf("2. Retiro.\n");
            printf("3. Transferencia.\n");
            printf("4. Regresar.\n");
            printf("\nOpcion: ");
            scanf("%d", &opcion);
            ENDL;
        } while (opcion <= 0 || opcion >= 5);

        switch (opcion)
        {
        case 1:
            Deposito(pathTransacciones, pathCuentas);
            break;
        case 2:
            Retiro(pathTransacciones, pathCuentas);
            break;
        case 3:
            Transferencia(pathTransacciones, pathCuentas);
            break;
        case 4:
            break;
        }
    } while (opcion != 4);
}

Transaccion getFileInfoTransaccion(const char *path)
{
    ulli_t nElements = getFileSize(path) / (ulli_t)sizeof(struct str_transaccion);
    Transaccion newArr = (Transaccion)malloc(nElements * sizeof(struct str_transaccion));
    FILE *file = fopen(path, "rb");
    if (file)
    {
        fread(newArr, sizeof(struct str_transaccion), nElements, file);
    }
    else
        printf("Error al abrir el archivo...\n");
    fclose(file);
    return newArr;
}

void writeAccFile(Cuenta cuentas, const char* path, ulli_t nElements){
    FILE *f = fopen(path, "wb");
    fwrite(cuentas, sizeof(struct str_cuenta), nElements, f);
    fclose(f);
}

void writeTranFile(Transaccion t, const char* path){
    //esta función recibe un apuntador tipo transaccion y el path del archivo transacciones
    FILE *f = fopen(path, "ab");
    //escribir en el archivo
    fwrite(t, sizeof(struct str_transaccion), 1, f);
    fclose(f);
}

bool validateAccount(const char* path, id_t id, double saldo){  // si no se va a validar saldo, se pone un cero como parámetro
    bool accountExists = false;
    bool hasMoney = false;
    ulli_t i;
    ulli_t nElements = getFileSize(path) / (ulli_t)sizeof(struct str_cuenta);
    Cuenta newArrAccount = (Cuenta)calloc(nElements, sizeof(struct str_cuenta));
    FILE *f = fopen(path, "rb");
    if(f){
        fread(newArrAccount, sizeof(struct str_cuenta), nElements, f);
    }else printf("Error al abrir el archivo Cuentas\n");
    fclose(f);

    for(i=0; i<nElements; i++){
        if(newArrAccount[i].id_cuenta == id){
            accountExists = true;
            break;
        }// terminar de validar la cuenta y despues validar que tenga saldo suficiente
    }
    if(newArrAccount[i].saldo >= saldo){
        hasMoney = true;
    }
    free(newArrAccount);    //liberar la memoria utilizada para el arreglo
    if(accountExists){
        if(hasMoney){
            return true;
        }
        return nem;
    }
    return false;
}

void Deposito(const char* PATH_TRANSACCIONES, const char* PATH_CUENTA){
    id_t cuenta;
    double monto;
    printf("id cuenta: ");
    scanf("%llu", &cuenta);
    ENDL;
    printf("monto a depositar: ");
    scanf("%lf", &monto);
    ENDL;

    ulli_t nElements = getFileSize(PATH_TRANSACCIONES) / (ulli_t)sizeof(struct str_transaccion);
    Transaccion nueva = (Transaccion)calloc(1, sizeof(struct str_transaccion));
    Transaccion newTranArr = getFileInfoTransaccion(PATH_TRANSACCIONES);
    bool accountExists = false;
    //Validar que la cuenta existe
    if(validateAccount(PATH_CUENTA, cuenta, 0)){
        //incrementar el saldo de la cuenta
        ulli_t i;
        ulli_t nElementsAcc = getFileSize(PATH_CUENTA) / (ulli_t)sizeof(struct str_cuenta);
        Cuenta newAccArr = (Cuenta)calloc(nElementsAcc, sizeof(struct str_cuenta));
        newAccArr = getFileInfoCuenta(PATH_CUENTA);
        for(i=0; i<nElementsAcc; i++){
            if(newAccArr[i].id_cuenta == cuenta){
                newAccArr[i].saldo = newAccArr[i].saldo + monto;
            }
        }
        writeAccFile(newAccArr, PATH_CUENTA, nElementsAcc);
        accountExists = true;
        free(newAccArr);
    }
    if(accountExists){
        //agregar nueva transacción (Depósito) al archivo
        if(!getFileSize(PATH_TRANSACCIONES)){
            nueva->id_transaccion = 1;
        }else{
            nueva->id_transaccion = newTranArr[nElements-1].id_transaccion + 1;
        }
        nueva->tipo_transaccion = strdup("Deposito");
        nueva->id_cuenta_origen = (id_t)0;
        nueva->id_cuenta_destino = cuenta;
        nueva->fecha = getDate();
        nueva->monto = monto;
        writeTranFile(nueva, PATH_TRANSACCIONES);
        free(nueva);
        free(newTranArr);
    }else{
        printf("La cuenta no existe.\n\n");
    }
}

void Retiro(const char* PATH_TRANSACCIONES, const char* PATH_CUENTA){
    id_t cuenta;
    double monto;
    printf("id cuenta: ");
    scanf("%llu", &cuenta);
    ENDL;
    printf("monto a retirar: ");
    scanf("%lf", &monto);
    ENDL;

    ulli_t nElements = getFileSize(PATH_TRANSACCIONES) / (ulli_t)sizeof(struct str_transaccion);
    Transaccion newTran = (Transaccion)calloc(1, sizeof(struct str_transaccion));
    Transaccion newTranArr = getFileInfoTransaccion(PATH_TRANSACCIONES);
    bool accountExists = validateAccount(PATH_CUENTA, cuenta, monto);
    if(accountExists == true){    //Validar que la cuenta existe
        //disminuir el saldo de la cuenta
        ulli_t i;
        ulli_t nElementsAcc = getFileSize(PATH_CUENTA) / (ulli_t)sizeof(struct str_cuenta);
        Cuenta newAccArr = (Cuenta)calloc(nElementsAcc, sizeof(struct str_cuenta));
        newAccArr = getFileInfoCuenta(PATH_CUENTA);
        for(i=0; i<nElementsAcc; i++){
            if(newAccArr[i].id_cuenta == cuenta){
                newAccArr[i].saldo = newAccArr[i].saldo - monto;
            }
        }
        //sobreescribir el archivo de cuenta
        writeAccFile(newAccArr, PATH_CUENTA, nElementsAcc);
        free(newAccArr);
        //agregar nueva transaccion (Retiro) al archivo
        if(!getFileSize(PATH_TRANSACCIONES)){
            newTran->id_transaccion = 1;
        }else{
            newTran->id_transaccion = newTranArr[nElements-1].id_transaccion + 1;
        }
        newTran->tipo_transaccion = strdup("Retiro");
        newTran->id_cuenta_origen = cuenta;
        newTran->id_cuenta_destino = (id_t)0;
        newTran->fecha = getDate();
        newTran->monto = monto;
        writeTranFile(newTran, PATH_TRANSACCIONES);
        free(newTran);
        free(newTranArr);
    }else if(accountExists == nem){
        printf("La cuenta no tiene fondos suficientes.\n\n");
    }else{
        printf("La cuenta no existe.\n\n");
    }
}

void Transferencia(const char* PATH_TRANSACCIONES, const char* PATH_CUENTA){
    id_t origen, destino;
    double monto;
    printf("id cuenta origen: ");
    scanf("%llu", &origen);
    ENDL;
    printf("id cuenta destino: ");
    scanf("%llu", &destino);
    ENDL;
    printf("monto a transferir: ");
    scanf("%lf", &monto);
    ENDL;
    ulli_t nElements = getFileSize(PATH_TRANSACCIONES) / (ulli_t)sizeof(struct str_transaccion);
    Transaccion newTran = (Transaccion)calloc(1, sizeof(struct str_transaccion));
    Transaccion newTranArr = getFileInfoTransaccion(PATH_TRANSACCIONES);
    bool OriginAccountExists = validateAccount(PATH_CUENTA, origen, monto);//Verificar que la cuenta origen tiene fondos suficientes
    bool TargetAccountExists = validateAccount(PATH_CUENTA, destino, 0);        //Verificar que la cuenta destino existe
    if((OriginAccountExists == true) && TargetAccountExists){     //Validar que ambas cuentas existen
        //disminuir el saldo de la cuenta origen
        ulli_t i;
        ulli_t nElementsAcc = getFileSize(PATH_CUENTA) / (ulli_t)sizeof(struct str_cuenta);
        Cuenta newAccArr = (Cuenta)calloc(nElementsAcc, sizeof(struct str_cuenta));
        newAccArr = getFileInfoCuenta(PATH_CUENTA);
        for(i=0; i<nElementsAcc; i++){
            if(newAccArr[i].id_cuenta == origen){
                newAccArr[i].saldo = newAccArr[i].saldo - monto;
            }
            if(newAccArr[i].id_cuenta == destino){
                newAccArr[i].saldo = newAccArr[i].saldo + monto;
            }
        }
        writeAccFile(newAccArr, PATH_CUENTA, nElementsAcc);
        if(!getFileSize(PATH_TRANSACCIONES)){
            newTran->id_transaccion = 1;
        }else{
            newTran->id_transaccion = newTranArr[nElements-1].id_transaccion + 1;
        }
        newTran->tipo_transaccion = strdup("Transferencia");
        newTran->id_cuenta_origen = origen;
        newTran->id_cuenta_destino = destino;
        newTran->fecha = getDate();
        newTran->monto = monto;
        writeTranFile(newTran, PATH_TRANSACCIONES);     //agregar nueva transacción (Transferencia) al archivo
        free(newTran);
        free(newTranArr);
    }else if(TargetAccountExists && (OriginAccountExists == nem)){
        printf("La cuenta origen no tiene fondos suficientes para realizar la transferencia.\n");
    }else if((TargetAccountExists == false) && OriginAccountExists){
        printf("La cuenta destino no existe.\n");
    }else{
        printf("La cuenta origen no existe.\n");
    }
}
#pragma endregion Transacciones
