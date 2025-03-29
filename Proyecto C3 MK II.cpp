#include <iostream>
#include <string>
#include <iomanip>
#include<conio.h>
#include <cctype>
#include<fstream>
using namespace std;

const int MAX_PRODUCTOS = 25;
const int LIMITE_INVENTARIO_TOTAL = 25;

string aMinusculas(string texto) {
    for (int i = 0; i < (int)texto.length(); i++) {
        texto[i] = tolower(texto[i]);
    }
    return texto;
}

int calcularTotalInventario(int inventario[][2], int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += (inventario[i][0] - inventario[i][1]);
    }
    return total;
}

// Función para mostrar el inventario
void mostrarInventario(string productos[], int inventario[][2], int n) {
    cout << "\n--- Inventario de productos ---\n";
    cout << left << setw(15) << "Producto" << setw(10) << "Cantidad" << setw(10) << "Salidas" << endl;
    cout << "----------------------------------------\n";
    for (int i = 0; i < n; i++) {
        int cantidadDisponible = inventario[i][0] - inventario[i][1]; // Entradas - Salidas
        if (cantidadDisponible < 0) cantidadDisponible = 0; // evitar mostrar números negativos
        cout << left << setw(15) << productos[i]
             << setw(10) << cantidadDisponible
             << setw(10) << inventario[i][1] << endl;
    }
}

void guardarInventarioEnArchivo(string productos[], int inventario[][2], int n) {
    ofstream archivo("inventario.txt");
    if (archivo.is_open()) {
        archivo << left << setw(15) << "Producto"
                << setw(10) << "Entradas"
                << setw(10) << "Salidas"
                << setw(10) << "Total" << endl;
        archivo << "---------------------------------------------------\n";
        for (int i = 0; i < n; i++) {
            int total = inventario[i][0] - inventario[i][1];
            if (total < 0) total = 0;
            archivo << left << setw(15) << productos[i]
                    << setw(10) << inventario[i][0]
                    << setw(10) << inventario[i][1]
                    << setw(10) << total << endl;
        }
        archivo.close();
        cout << "\nInventario guardado en el archivo 'inventario.txt'\n";
    } else {
        cout << "\nError al crear el archivo.\n";
    }
}

// Función para agregar entrada o salida
void actualizarInventario(int inventario[][2], int pos, int cantidad, bool esEntrada) {
    if (esEntrada) {
        inventario[pos][0] += cantidad;
    } else {
        inventario[pos][1] += cantidad;
    }
}

// Función de búsqueda en arreglo unidimensional
int buscarProducto(string productos[], int n, string valorBuscado) {
    valorBuscado = aMinusculas(valorBuscado); // Convertir valor buscado a minúsculas

    int posicion = -1;
    for (int i = 0; i < n; i++) {
        if (aMinusculas(productos[i]) == valorBuscado) { // Comparar en minúsculas
            posicion = i;
        }
    }
    return posicion;
}

void buscarProductoPorNombre(string productos[], int inventario[][2], int n, string valorBuscado) {
    valorBuscado = aMinusculas(valorBuscado);
    int bandera = 0;
    for (int i = 0; i < n; i++) {
        if (aMinusculas(productos[i]) == valorBuscado) { // Comparar ambos en minúsculas
            int cantidadActual = inventario[i][0] - inventario[i][1];
            cout << "\nProducto encontrado: " << productos[i] << endl;
            cout << "Cantidad disponible: " << cantidadActual << endl;
            bandera = 1;
            i = n;
        }
    }
    if (bandera == 0) {
        cout << "Producto no encontrado.\n";
    }
}


int main() {
    string productos[MAX_PRODUCTOS] = {"Jabon", "Cloro", "Escoba", "Trapeador", "Desinfectante"};
    int inventario[MAX_PRODUCTOS][2] = {0}; // columna 0: entradas, columna 1: salidas
    int opcion, cantidad, cantidadProductos = 5;
    string nombreProducto;
    char continuar = 's';
    do {
        cout << "\n--- Menu de Inventario ---\n";
        cout << "1. Ver inventario\n";
        cout << "2. Agregar producto\n";
        cout << "3. Quitar producto\n";
        cout << "4. Buscar producto\n";
        cout << "5. Agregar nuevo articulo\n";
        cout << "6. Eliminar producto\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        do {
            cout << "Seleccione una opcion (0 a 6): ";
            cin >> opcion;
            if (opcion < 0 || opcion > 6) {
                cout << "Opcion invalida. Intente de nuevo.\n";
            }
        } while (opcion < 0 || opcion > 6);

        switch (opcion) {
            case 1:
                mostrarInventario(productos, inventario, cantidadProductos);
                guardarInventarioEnArchivo(productos, inventario, cantidadProductos);
                break;
                case 2: {
                    cout << "Nombre del producto: ";
                    cin >> nombreProducto;
                    nombreProducto = aMinusculas(nombreProducto);
                
                    do {
                        cout << "Cantidad a ingresar: ";
                        cin >> cantidad;
                        if (cantidad < 0) {
                            cout << "No se permiten cantidades negativas.\n";
                        }
                    } while (cantidad < 0);
                
                    int pos = buscarProducto(productos, cantidadProductos, nombreProducto);
                    if (pos != -1) {
                        int totalActual = calcularTotalInventario(inventario, cantidadProductos);
                        if (totalActual + cantidad > LIMITE_INVENTARIO_TOTAL) {
                            cout << "No se puede agregar. Se superaría el límite total de 25 unidades.\n";
                        } else {
                            actualizarInventario(inventario, pos, cantidad, true);
                        }
                    } else {
                        cout << "Producto no encontrado.\n";
                    }
                    break;
                }                
                case 3: {
                    cout << "Nombre del producto: ";
                    cin >> nombreProducto;
                    nombreProducto = aMinusculas(nombreProducto);
                
                    do {
                        cout << "Cantidad a sacar: ";
                        cin >> cantidad;
                        if (cantidad < 0) {
                            cout << "No se permiten cantidades negativas.\n";
                        }
                    } while (cantidad < 0);
                
                    int pos = buscarProducto(productos, cantidadProductos, nombreProducto);
                    if (pos != -1) {
                        int disponible = inventario[pos][0] - inventario[pos][1];
                        if (cantidad > disponible) {
                            cout << "No hay suficientes unidades para sacar. Solo hay " << disponible << ".\n";
                        } else {
                            actualizarInventario(inventario, pos, cantidad, false);
                        }
                    } else {
                        cout << "Producto no encontrado.\n";
                    }
                    break;
                }                
            case 4:
                cout << "Nombre del producto a buscar: ";
                cin >> nombreProducto;
                buscarProductoPorNombre(productos, inventario, cantidadProductos, nombreProducto);
                break;
            
            case 5:
                if (cantidadProductos >= MAX_PRODUCTOS) {
                    cout << "Límite de productos alcanzado. No se pueden agregar más.\n";
                    break;
                }
            
                cout << "Ingrese el nombre del nuevo producto: ";
                cin >> nombreProducto;
                nombreProducto = aMinusculas(nombreProducto);
            
                // Verificar que no exista ya
                if (buscarProducto(productos, cantidadProductos, nombreProducto) != -1) {
                    cout << "El producto ya existe en el inventario.\n";
                    break;
                }
            
                productos[cantidadProductos] = nombreProducto;
                inventario[cantidadProductos][0] = 0; // Entradas
                inventario[cantidadProductos][1] = 0; // Salidas
                cantidadProductos++;
            
                cout << "Producto agregado correctamente.\n";
                break;
                case 6: {
                    cout << "Nombre del producto a eliminar: ";
                    cin >> nombreProducto;
                    nombreProducto = aMinusculas(nombreProducto);
                    
                    int pos = buscarProducto(productos, cantidadProductos, nombreProducto);
                    if (pos != -1) {
                        for (int i = pos; i < cantidadProductos - 1; i++) {
                            productos[i] = productos[i + 1];
                            inventario[i][0] = inventario[i + 1][0];
                            inventario[i][1] = inventario[i + 1][1];
                        }
                        cantidadProductos--;
                        cout << "Producto eliminado correctamente.\n";
                    } else {
                        cout << "Producto no encontrado.\n";
                    }
                    break;
                }                
            case 0:
                cout << "Saliendo del programa...\n";
                guardarInventarioEnArchivo(productos, inventario, cantidadProductos);
                return 0;
            default:
                cout << "Opcion invalida.\n";
                break;
        }
        cout << "\nDesea realizar otra operacion? (s/n): ";
        cin >> continuar;
        if (continuar == 'n' || continuar == 'N') {
            cout << "Gracias por usar el sistema de inventario.\n";
            break;
        }
    } while (continuar == 's' || continuar == 'S');
    getch();
    return 0;
}