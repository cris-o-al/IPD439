import serial
import time

# Configuración del puerto
PORT = 'COM3' 
BAUD = 115200

def get_checksum(data):
    """Calcula el checksum mediante XOR de todos los bytes."""
    cs = 0
    for b in data:
        cs ^= b
    return cs


# Inicializar Puerto (8 bits, Paridad Par, 1 Stop bit)
ser = serial.Serial(PORT, BAUD, parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE, timeout=2)
    
print(f"--- Iniciando lectura de Flash en {PORT} ---")
    
# Sincronización
ser.write(b'\x7F')
if ser.read(1) == b'\x79':
    print("[OK] Sincronizado")
else:
    print("[ERR] No se recibió ACK de sincronización")
    exit()

# Comando Read Memory (0x11 + Checksum 0xEE)
ser.write(b'\x11\xEE')
if ser.read(1) == b'\x79':
        print("[OK] Comando Read Memory aceptado")
    
# Enviar Dirección (0x08000000) + Checksum
addr = b'\x08\x00\x00\x00'
cs_addr = get_checksum(addr)
ser.write(addr + bytes([cs_addr]))
    
if ser.read(1) == b'\x79':
    print("[OK] Dirección 0x08000000 aceptada")

# Enviar cantidad de bytes (N-1). Para 256 bytes enviamos 255 (0xFF)
# Checksum de N-1 es su complemento: ~0xFF = 0x00
ser.write(b'\xFF\x00')
    
if ser.read(1) == b'\x79':
    print("[OK] Cantidad aceptada. Recibiendo datos...\n")
        
    # Leer los 256 bytes resultantes
    data = ser.read(256)

    print("--- Datos en crudo (Orden Físico) ---")
    for i in range(0, 32, 16): # Se muestran solo las primeras dos filas de ejemplo
        chunk = data[i:i+16]
        hex_raw = ' '.join(f'{b:02X}' for b in chunk)
        print(f"0x{0x08000000 + i:08X}: {hex_raw}")

    print("\n--- Datos formateados (Words de 32 bits) ---")
    for i in range(0, len(data), 16):
        row = data[i:i+16]
        words = []
        # Se agrupan de a 4 bytes y reordenan en formato Little-Endian
        for j in range(0, len(row), 4):
            word_chunk = row[j:j+4]
            val = int.from_bytes(word_chunk, byteorder='little')
            words.append(f"{val:08X}")
        
        print(f"0x{0x08000000 + i:08X}: {'  '.join(words)}")
