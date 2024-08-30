# 75.41/95.15: Algoritmos y Programacion I - Catedra Mendez - 2°C 2023
# TP3

## Descripción:

Blancanieves y los enanitos, tras el éxito en la venta de sus cosechas, necesitan un sistema para gestionar sus pedidos de manera eficiente.

El sistema debe gestionar dos archivos CSV: uno para pedidos y otro para clientes. Cada pedido incluye un ID, tipo de verdura y cantidad. Los pedidos y clientes están relacionados por el ID del pedido. Se debe manejar errores y proporcionar retroalimentación clara al usuario, asegurando que todos los datos se mantengan correctos y consistentes en los archivos.

Los comandos disponibles son:
- **Agregar/**: Añadir un nuevo pedido con la sintaxis agregar <cantidad> <verdura> <nombre cliente>.
- **Modificar/**: Actualizar un pedido existente con la sintaxis modificar <id_pedido> <cantidad> <verdura>.
- **Eliminar/**: Eliminar un pedido y todas sus entradas asociadas con la sintaxis eliminar <id_pedido>.
- **Listar/**: Mostrar todos los pedidos o un pedido específico con la sintaxis listar [id_pedido].

## Comandos para el TP3

El sistema de gestión de pedidos se controla mediante una serie de comandos. Deben seguir el siguiente formato:

python3 verduleria.py <comando> <parámetros>
