package types

import (
	"icu/models"
	"time"
)

type IPGeo struct {
	IP                  string   `json:"ip"`
	ContinentCode       string   `json:"continent_code"`
	ContinentName       string   `json:"continent_name"`
	CountryCode2        string   `json:"country_code2"`
	CountryCode3        string   `json:"country_code3"`
	CountryName         string   `json:"country_name"`
	CountryNameOfficial string   `json:"country_name_official"`
	CountryCapital      string   `json:"country_capital"`
	StateProv           string   `json:"state_prov"`
	StateCode           string   `json:"state_code"`
	District            string   `json:"district"`
	City                string   `json:"city"`
	Zipcode             string   `json:"zipcode"`
	Latitude            float64  `json:"latitude,string"`
	Longitude           float64  `json:"longitude,string"`
	IsEu                bool     `json:"is_eu"`
	CallingCode         string   `json:"calling_code"`
	CountryTLD          string   `json:"country_tld"`
	Languages           string   `json:"languages"`
	CountryFlag         string   `json:"country_flag"`
	GeonameID           uint64   `json:"geoname_id,string"`
	Isp                 string   `json:"isp"`
	ConnectionType      string   `json:"connection_type"`
	Organization        string   `json:"organization"`
	CountryEmoji        string   `json:"country_emoji"`
	Currency            Currency `json:"currency"`
	TimeZone            TimeZone `json:"time_zone"`
}

type Currency struct {
	Code   string `json:"code"`
	Name   string `json:"name"`
	Symbol string `json:"symbol"`
}

type TimeZone struct {
	Name            string  `json:"name"`
	Offset          int     `json:"offset"`
	OffsetWithDst   int     `json:"offset_with_dst"`
	CurrentTime     string  `json:"current_time"`
	CurrentTimeUnix float64 `json:"current_time_unix"`
	IsDst           bool    `json:"is_dst"`
	DstSavings      int     `json:"dst_savings"`
	DstExists       bool    `json:"dst_exists"`
	DstStart        string  `json:"dst_start"`
	DstEnd          string  `json:"dst_end"`
}

type TCPCache struct {
	Unique       string
	Cache        interface{}
	LastActivity time.Time
}

type TCPSession struct {
	Bot          models.IBot
	LastActivity time.Time
}
