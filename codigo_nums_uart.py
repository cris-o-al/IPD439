import serial
import time
import random
import statistics
# --- CONFIGURACIÓN ---
PORT = 'COM3'  
BAUD = 115200
TIMEOUT = 5    

try:
    # Abrir el puerto serial
    ser = serial.Serial(PORT, BAUD, timeout=TIMEOUT)
    print(f"--- Conectado a {PORT} ---")
    time.sleep(1)

    # 100 números reales aleatorios
    numeros = [round(random.uniform(1.0, 100.0), 2) for _ in range(100)]
    
    # Calcular valores esperados en Python para comparar
    promedio_esperado = sum(numeros) / 100
    desv_std_esperada = statistics.pstdev(numeros) # pstdev usa N=100 igual que tu código en el micro

    print(f"Enviando 100 números.")
    print(f"Esperado -> Promedio: {promedio_esperado:.4f}, Desv. Est: {desv_std_esperada:.4f}")

    # Enviar los números
    for i, n in enumerate(numeros):
        mensaje = f"{n}\n"
        ser.write(mensaje.encode('ascii'))
        
        time.sleep(0.01) 
        
        if (i + 1) % 10 == 0:
            print(f"Enviados {i+1}/100...")

    print("--- Envío completado. Esperando respuesta del STM32... ---")

    # 
    # Leemos línea por línea hasta que el puerto se cierre o termine el timeout
    while True:
        linea = ser.readline().decode('ascii').strip()
        if linea:
            print(f"[STM32]: {linea}")
        else:
            # Si readline devuelve vacío es porque se acabó el timeout
            break

    ser.close()
    print("--- Prueba finalizada ---")

except serial.SerialException as e:
    print(f"Error al abrir el puerto: {e}")
except KeyboardInterrupt:
    print("\nPrueba cancelada por el usuario.")
