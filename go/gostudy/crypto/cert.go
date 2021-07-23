package main

import (
	"bufio"
	"crypto"
	"crypto/rand"
	"crypto/rsa"
	"crypto/sha256"
	"crypto/x509"
	"encoding/pem"
	"errors"
	"fmt"
	"io"
	"log"
	"os"
	"strconv"
	"strings"
)

func main() {
	genRsaKeys(16, 8)
}

func GetPeerIps() ([]string, error) {
	file, err := os.OpenFile("node.txt", os.O_RDONLY, 0444)
	if err != nil {
		log.Panic(err)
	}

	ips := []string{}

	buf := bufio.NewReader(file)
	for {
		line, err := buf.ReadString('\n')
		line = strings.TrimSpace(line)
		ips = append(ips, line)
		if err == io.EOF {
			break
		} else if err != nil {
			log.Panic(err)
		}
	}
	return ips, nil
}

func Ip2I64(ip string) int64 {
	res := int64(0)

	for _, span := range strings.Split(ip, ".") {
		num, err := strconv.Atoi(span)
		if err != nil {
			log.Panic(err)
			return 0
		}
		res = res*1000 + int64(num)
	}
	return res
}

//如果当前目录下不存在目录Keys，则创建目录，并为各个节点生成rsa公私钥
func genRsaKeys(ipNum, processNum int) {
	if !isExist("./certs") {
		fmt.Println("检测到还未生成公私钥目录，正在生成公私钥 ...")
		err := os.Mkdir("certs", 0644)
		if err != nil {
			log.Panic(err)
		}
		PeerIPs, err := GetPeerIps()
		if err != nil {
			log.Panic(err)
		}
		for _, ip := range PeerIPs[:ipNum] {
			idPrefix := Ip2I64(ip)
			for i := 1; i <= processNum; i++ {

				id := idPrefix*int64(100) + int64(i)
				keyDir := "./certs/" + fmt.Sprint(id)
				if !isExist(keyDir) {
					err := os.Mkdir(keyDir, 0644)
					if err != nil {
						log.Panic()
					}
				}
				pri, pub := getKeyPair()
				priFilePath := keyDir + "/rsa.pri.pem"
				priFile, err := os.OpenFile(priFilePath, os.O_RDWR|os.O_CREATE, 0644)
				if err != nil {
					log.Panic(err)
				}
				defer priFile.Close()
				priFile.Write(pri)

				pubFilePath := keyDir + "/rsa.pub.pem"
				pubFile, err := os.OpenFile(pubFilePath, os.O_RDWR|os.O_CREATE, 0644)
				if err != nil {
					log.Panic(err)
				}
				defer pubFile.Close()
				pubFile.Write(pub)

				addr := ip + "." + strconv.Itoa(8000+i)
				addrFilePath := keyDir + "/" + addr + ".addr.txt"
				addrFile, err := os.OpenFile(addrFilePath, os.O_RDWR|os.O_CREATE, 0644)
				if err != nil {
					log.Panic(err)
				}
				defer addrFile.Close()
				addrFile.WriteString(ip + ":" + strconv.Itoa(8000+i))
			}
		}
		fmt.Println("已为节点们生成RSA公私钥")
	}
}

//生成rsa公私钥
func getKeyPair() (prvkey, pubkey []byte) {
	// 生成私钥文件
	privateKey, err := rsa.GenerateKey(rand.Reader, 1024)
	if err != nil {
		panic(err)
	}
	derStream := x509.MarshalPKCS1PrivateKey(privateKey)
	block := &pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: derStream,
	}
	prvkey = pem.EncodeToMemory(block)
	publicKey := &privateKey.PublicKey
	derPkix, err := x509.MarshalPKIXPublicKey(publicKey)
	if err != nil {
		panic(err)
	}
	block = &pem.Block{
		Type:  "PUBLIC KEY",
		Bytes: derPkix,
	}
	pubkey = pem.EncodeToMemory(block)
	return
}

//判断文件或文件夹是否存在
func isExist(path string) bool {
	_, err := os.Stat(path)
	if err != nil {
		if os.IsExist(err) {
			return true
		}
		if os.IsNotExist(err) {
			return false
		}
		fmt.Println(err)
		return false
	}
	return true
}

type pbft struct{}

//数字签名
func (p *pbft) RsaSignWithSha256(data []byte, keyBytes []byte) []byte {
	h := sha256.New()
	h.Write(data)
	hashed := h.Sum(nil)
	block, _ := pem.Decode(keyBytes)
	if block == nil {
		panic(errors.New("private key error"))
	}
	privateKey, err := x509.ParsePKCS1PrivateKey(block.Bytes)
	if err != nil {
		fmt.Println("ParsePKCS8PrivateKey err", err)
		panic(err)
	}

	signature, err := rsa.SignPKCS1v15(rand.Reader, privateKey, crypto.SHA256, hashed)
	if err != nil {
		fmt.Printf("Error from signing: %s\n", err)
		panic(err)
	}

	return signature
}

//签名验证
func (p *pbft) RsaVerySignWithSha256(data, signData, keyBytes []byte) bool {
	block, _ := pem.Decode(keyBytes)
	if block == nil {
		panic(errors.New("public key error"))
	}
	pubKey, err := x509.ParsePKIXPublicKey(block.Bytes)
	if err != nil {
		panic(err)
	}

	hashed := sha256.Sum256(data)
	err = rsa.VerifyPKCS1v15(pubKey.(*rsa.PublicKey), crypto.SHA256, hashed[:], signData)
	if err != nil {
		panic(err)
	}
	return true
}
