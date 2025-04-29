package utils

func CRC8XOR(data []byte) byte {
	var crc byte
	for _, b := range data {
		crc ^= b
	}

	return crc
}

func CheckCRC(buffer []byte) bool {
	if len(buffer) < 2 {
		return false
	}

	data := buffer[:len(buffer)-1]
	expectedCRC := buffer[len(buffer)-1]
	actualCRC := CRC8XOR(data)

	return actualCRC == expectedCRC
}
