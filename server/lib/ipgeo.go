package lib

import (
	"fmt"
	"icu/common/types"
	"io/ioutil"
	"net"
	"net/http"
	"os"

	jsoniter "github.com/json-iterator/go"
)

var json = jsoniter.ConfigCompatibleWithStandardLibrary

func GetIPGeo() *types.IPGeo {
	logger := NewLogger()
	url := fmt.Sprintf("https://api.ipgeolocation.io/ipgeo?apiKey=%s", os.Getenv("API_IPGEO"))

	resp, err := http.Get(url)
	if err != nil {
		logger.Error(err.Error())
		return &types.IPGeo{}
	}
	defer resp.Body.Close()

	bytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		logger.Error(fmt.Sprintf("Failed to read response ipgeo: %s", err.Error()))
		return &types.IPGeo{}
	}

	var ipgeo types.IPGeo
	if err := json.Unmarshal(bytes, &ipgeo); err != nil {
		logger.Error(fmt.Sprintf("Failed to unmarshal JSON: %s", err.Error()))
		return &types.IPGeo{}
	}

	return &ipgeo
}

func GetMAC() string {
	logger := NewLogger()

	interfaces, err := net.Interfaces()
	if err != nil {
		logger.Error(fmt.Sprintf("Failed get mac-address %s", err.Error()))
		return ""
	}

	for _, iface := range interfaces {
		if iface.HardwareAddr.String() != "" {
			return iface.HardwareAddr.String()
		}
	}

	return ""
}
