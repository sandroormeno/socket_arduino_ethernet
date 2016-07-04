var http = require('http'); 
var url = require('url'); 
var TEMP_LIMIT = 28; 
// Definimos el servidor HTTP 
// La función interna es conocida como `request handler`, y es ejecutada 
// cada vez que una peticion `http` llega al servidor 
var server = http.createServer(function (req, res) { 
  // Obtenemos la `url` solicitada por el cliente, esta `url` contiene 
  // los parámetros de la petición, luego necesitamos analizar (parsear) su contenido 
var endpoint = req.url; 
// Verificamos si la URL contiene la ruta `/temp` 
if (endpoint.indexOf('/temp') !== -1) { 
  // Analizamos la URL para extraer su contenido 
  var parsedUrl = url.parse(endpoint, true); 
  // Obtenemos el objeto que contiene los parámetros 
  var params = parsedUrl.query || {}; 
  // Obtenemos la temperatura o `null` si no tenemos ningún valor 
  var temp = params.value || null; 
  // Realizamos la validación de temperatura y retornamos el resultado al cliente 
  if (temp) { 
    if (temp > TEMP_LIMIT) { 
      res.write('|warning|'); 
      } else { res.write('|normal|');
    }
  } else { res.write('Wrong param, please use `value`'); 
  } 
  } else { res.write('Wrong endpoint, please use `/temp`'); 
        
  } res.end(); 
  
}); 
// Ejecutamos nuestro servidor en el puerto 8080 
server.listen(8080);
     