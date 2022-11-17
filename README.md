PAV - P3: estimación de pitch
=============================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la estimación de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.
      
	<pre><code>  for (unsigned int l = 0; l < r.size(); ++l) {
		/// \TODO Compute the autocorrelation r[l]
	      	/// \FET Hem computat la autocorrelació fent ús de la fórmula donada al enunciat: 
	      	/// r[l] = sum( x[n] x[n+l] ) desde n=0 a n=N-l
		for (unsigned int n = 0; n < x.size()-l; n++) {
			r[l] += x[n]*x[n+l];
		}
	}
	
	if (r[0] == 0.0F) //to avoid log() and divide zero 
		r[0] = 1e-10;
      </code></pre>
      
   * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un segmento de
     unos 30 ms de un fonema sonoro y su periodo de pitch; y, en otro *subplot*, se vea con claridad la
	   autocorrelación de la señal y la posición del primer máximo secundario.

	 NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
	 hacerlo. Se valorará la utilización de la biblioteca matplotlib de Python.

      <img width="1366" alt="Plot senyal i autocorrelació" src="https://github.com/saratarratss/P3/blob/Estevez-Mesquida-Tarrats/PAV3capt1.png">

      *Hem seleccionat 30 ms a partir dels 750ms del fitxer prueba.wav.*

   * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la
     autocorrelación. Inserte a continuación el código correspondiente.

	<pre><code>  vector<float>::const_iterator iR = r.begin(), iRMax = iR;

	for(iR = iRMax =  r.begin() + npitch_min; iR < r.begin() + npitch_max; iR++){  
      		//r.begin para empezar por el primer valor de la autocorrelacion
		if(*iR > * iRMax) {
			iRMax = iR;
		}
	}</pre></code>
	
   * Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.

	<pre><code>  bool unvoiced = true;
	if (rmaxnorm > umaxnorm || r1norm>0.97){
		unvoiced = false;
	}
	if (pot<-20 ){
		unvoiced = true;
	}
	return unvoiced;</pre></code>

   * Puede serle útil seguir las instrucciones contenidas en el documento adjunto `código.pdf`.

- Una vez completados los puntos anteriores, dispondrá de una primera versión del estimador de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la estimación de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		(r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).

		Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	    Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

      <img width="1366" alt="Candidatos sonoridad" src="https://github.com/saratarratss/P3/blob/Estevez-Mesquida-Tarrats/PAV3capt8.PNG">

      *Hem representat utilitzant wavesurfer amb aquest ordre l'autocorrelació en un màxim secundari, l'autocorrelació normalitzada de 1 i el nivell de potència d'un senyal del fitxer prueba.wav. Dins l'interval de 15 segons (color groc) tant rmaxnorm té prenen un valor una mica superior a 0,8 i r1norm és una mínimament inferior a 1. El valor de la potència es de 0 dB i el mínim absolut d'aquesta és -100 dB.*

      - Use el estimador de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
	    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica
		ilustrativa del resultado de ambos estimadores.
     
		Aunque puede usar el propio Wavesurfer para obtener la representación, se valorará
	 	el uso de alternativas de mayor calidad (particularmente Python).
  
    <img width="1366" alt="Comparativa" src="https://github.com/saratarratss/P3/blob/Estevez-Mesquida-Tarrats/PAV3capt4.png">

    *En aquesta imatge veiem que els dos gràfics s'assemblen molt tot i tenir diferències ínfimes. La major diferència és el màxim que trobam a t=0 del pitch implementat en el programa per culpa de l'enfinestrat de Hamming.*

  * Optimice los parámetros de su sistema de estimación de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..

    <img width="1366" alt="Summary" src="https://github.com/saratarratss/P3/blob/Estevez-Mesquida-Tarrats/PAV3capt6.PNG">

Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  estimador a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del estimador. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la estimación de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.

  <img width="1366" alt="Summary" src="https://github.com/saratarratss/P3/blob/Estevez-Mesquida-Tarrats/PAV3capt7.PNG">

    *Hem afegit la variable umaxnorm que és el llindar màxim de la autocorrelació normalitzada*.

- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de estimación
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:

  * Técnicas de preprocesado: filtrado paso bajo, diezmado, *center clipping*, etc.
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el estimador, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al estimador. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.

  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   
    *Hem utilitzat dues tècniques diferents per optimitzar les característiques del sistema d'estimació de pitch*

    *La tècnica de preprocessat que hem utilitzat és el center clipping. Amb aquesta tecnica el que fem es retallar el senyal en amplitud per poder així a l'hora de introduïr distorsió no lineal augmentarà la intensitat dels harmònics d'ordre elevat. El codi és el següent:*

	<pre><code>  float alfa=0.006;
	for (iX = x.begin(); iX  < x.end(); iX++ ) {
		if (*iX < alfa && *iX > -alfa){ 
			*iX = 0;
		}
		</code></pre><p>
	*La tècnica de postprocessat que hem utilitzat és el filtre de mediana. Aquesta tècnica consisteix en calcular el valor mitjà amb una finestra tancada a cada instant de temps per aconseguir eliminar soroll. El còdi és el següent:*

    
		void median_filter(vector<float> &pitches){
			vector<float> sorted = pitches;
			vector<float> sorting = pitches;
			float a;
			for (unsigned int i = 1; i < pitches.size() - 1; i++){
				sorting[0] = pitches[i - 1];
				sorting[1] = pitches[i];
				sorting[2] = pitches[i + 1];
				for (int j = 0; j < 2; j++){
					for (int k = 0; k < 2; k++){
						if (sorting[k] > sorting[k + 1]){
							a = sorting[k + 1];
							sorting[k + 1] = sorting[k];
							sorting[k] = a;
						}
					}
				}
				sorted[i] = sorting[1];
			}
			pitches = sorted;
		}
		
	
  *Finalment hem jugat amb la longitud de la finestra del filtre per poder veure els efectes que provocava en la taxes d'error i el score TOTAL*.
 
	<img width="1150" alt="Diferents L del filtre" src="https://github.com/saratarratss/P3/blob/Estevez-Mesquida-Tarrats/PAV.png">

  *Veiem que a mesura que augmentem la grandària de la finestra, empitjora el *score* TOTAL significativament i la *MSE of fine errors* en menor mesura. També   observem que el percentatge de *Gross voiced errors* millora per a L=4 i 5 i torna a caure per a L=6 en endavant*.
  
	<pre><code>  void median_filter(vector<float> &pitches){
		vector<float> sorted = pitches;
		vector<float> sorting = pitches;
		float a;

		for (unsigned int i = 1; i < pitches.size() - 1; i++){
			sorting[0] = pitches[i - 1];
			sorting[1] = pitches[i];
			sorting[2] = pitches[i + 1];
			/*Aumentamos la dimensión de la ventana para comprobar su efecto*/
			//Se ha probado con cada uno
			sorting[3] = pitches[i + 2]; //l=4
			sorting[4] = pitches[i + 3];//L=5
			sorting[5] = pitches[i + 4];//L=6
			sorting[6] = pitches[i + 5];//L=7

			for (int j = 0; j < 6; j++){
				for (int k = 0; k < 6; k++){
					if (sorting[k] > sorting[k + 1]){
						a = sorting[k + 1];
						sorting[k + 1] = sorting[k];
						sorting[k] = a;
					}
				}
			}
			sorted[i] = sorting[1];
		}
		pitches = sorted;
	}</code></pre>
 
Evaluación *ciega* del estimador
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
estimador con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
