package main
  
import (
	//"./dataminer"
	//"./problem"
	"code.google.com/p/go.net/websocket"
	"flag"
	//"fmt"
	"log"
	"math/rand"
	"net/http"
	//"os"
	//"os/signal"
	//"runtime"
	"text/template"
	"time"
)

var addr = flag.String("addr", ":8080", "http service address")
var homeTempl = template.Must(template.ParseFiles("./home.html"))
//var attackTempl = template.Must(template.ParseFiles("./attack.html"))
//var jsTempl = template.Must(template.ParseFiles("./pow.js"))
//var jsAtkTempl = template.Must(template.ParseFiles("./pow_atk.js"))
//var jsAtkTaskTempl = template.Must(template.ParseFiles("./attacktask.js"))
//var monitorTempl = template.Must(template.ParseFiles("./monitor.html"))

var serveStaticMap = make(map[string]*template.Template)

func homeHandler(c http.ResponseWriter, req *http.Request) {
	homeTempl.Execute(c, 1 )
}
//func attackHandler(c http.ResponseWriter, req *http.Request) {
//	attackTempl.Execute(c, problem.BaseDifficulty)
//}
func ipHandler(c http.ResponseWriter, req *http.Request) {
	// c.WriteHeader()
	c.Write([]byte(req.RemoteAddr))
}

// func jsAtkHandler(c http.ResponseWriter, req *http.Request) {
// 	jsAtkTempl.Execute(c, req.Host)
// }

// func jsAtkTaskHandler(c http.ResponseWriter, req *http.Request) {
// 	jsAtkTaskTempl.Execute(c, req.Host)
// }

// func jsHandler(c http.ResponseWriter, req *http.Request) {
// 	jsTempl.Execute(c, req.Host)
// }
//func monitorHTMLHandler(c http.ResponseWriter, req *http.Request) {
///	monitorTempl.Execute(c, req.Host)
//}

func serveStatic(file string) {
	serveStaticMap[file] = template.Must(template.ParseFiles("./" + file))
	http.HandleFunc("/"+file, func(c http.ResponseWriter, req *http.Request) {
		serveStaticMap[file].Execute(c, req.Host)
	})
}

func main() {
	flag.Parse()
	//cpu := runtime.GOMAXPROCS(runtime.NumCPU())
	//log.Printf("PoW server started on %s server at %s, threads: %d -> %d\n", runtime.GOOS, *addr, cpu, runtime.NumCPU())
	rand.Seed(time.Now().UTC().UnixNano())

	http.HandleFunc("/", homeHandler)

	serveStatic("jquery.js")
	serveStatic("app.js")
	serveStatic("css/bootstrap.min.css")
	serveStatic("css/bootstrap-theme.min.css")
	serveStatic("css/slider.css")
	serveStatic("js/bootstrap.min.js")
	serveStatic("Touchscreen-Joystick/TouchControl.js")
	serveStatic("js/bootstrap-slider.js")
	
	http.Handle("/ws", websocket.Handler(wsHandler))
	//http.Handle("/monitor_ws", websocket.Handler(monitorHandler))
	if err := http.ListenAndServe(*addr, nil); err != nil {
		log.Fatal("ListenAndServe:", err)
	}
}
