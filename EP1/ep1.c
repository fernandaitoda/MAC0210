/********************************** 
		EP1 - MAC0210 
	
	    Fernanda Itoda
	       10740825
***********************************/


#include <stdio.h> 
#include <math.h>
#include <complex.h>
#include <GL/glut.h>

#define TOL 0.00001f

float Fx (float x)
{
	return exp (x) - 2 * pow (x, 2);
}

float Gx1 (float x) 
{
	return x - (Fx (x) / (exp (x) - 2 * 2 * x));
} 

float Gx2 (float x)
{
	return log (2 * pow (x, 2));
}

float pontoFixo (float x0, int n)
{ 
	float raiz;

	if (n == 1) 
		while (fabs (Fx (x0)) > TOL) {
			if (fabs (Fx (x0)) < TOL) {
				raiz = x0;
				break;
			}
			x0 = Gx1 (x0);
		}

	else
		while (fabs (Fx (x0)) > TOL) {
			if (fabs (Fx (x0)) < TOL) {
				raiz = x0;
				break;
			}
			x0 = Gx2 (x0);
	}

	return x0;
}

float complex evalf (float complex z) 
{
	return cpowf (z, 2) + 1;
	//return cpowf (z, 3) - z;
}

float complex evalDf (float complex z)
{
	return 2.0 * z;
	//return 3 * cpowf (z, 2) - 1;
}

void keyboard (unsigned char key, int x, int y){
  switch (key) {
  case 27:     // tecla Esc (encerra o programa)
	exit (0);
	break;
  }
}

void display ()
{
	float x, y;
	int cor;
	
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	FILE *arq = fopen ("output.txt", "r");
	
	
	while (!feof (arq)) {

		// lê a cor
		fscanf (arq, "%d", &cor);
		// lê o ponto inicial
		fscanf (arq, " %f %f ", &x, &y);
		/*
		*/
		glPointSize (10.0f);
		if (cor == 1) 
			glColor3f (1.0, 0.412, 0.706);
		else if (cor == 2)
			glColor3f (0.596, 0.984, 0.596);
        else if (cor == 3)
			glColor3f (0.098, 0.098, 0.439);
        else if (cor == 4)
            glColor3f (1.0, 0.271, 0.0);
        else if (cor == 5)
            glColor3f (1.0, 1.0, 0.0);
        else if (cor == 6)
		    glColor3f (0.596, 0.984, 0.596);
		else
            glColor3f (0.545, 0.0, 0.545);

		glBegin (GL_POINTS);
			glVertex2f (x, y);
		glEnd ();
		
		}
		glFlush ();
}


float complex newton (float complex z0)
{
	float complex raiz;

	while (cabsf (evalf (z0)) > TOL) {
		if (cabsf (evalf (z0)) < TOL) {
			raiz = z0;
			break;
		}
		z0 = z0 - evalf (z0) / evalDf (z0);
	}	

	return z0;
}

void newton_basins (float complex l, float complex u, float p)
{
	FILE *output;
	float complex z, raiz1, raiz2, raiz;
	z = l;

	output = fopen ("output.txt", "w");
	
	if (output)
		fclose (output);

	output = fopen ("output.txt", "w");
	
	if (!output) 
		printf ("Erro ao abrir arquivo.\n");
	else {
		printf ("Arquivo 'output.txt criado.\n");
			

		while (creal (z) <= creal (u)) {
			while (cimag (z) <= cimag (u)) {
				fprintf (output, "%.2f %.2f ", creal (z), cimag (z));
				raiz = newton (z);

				// cores
				// Para mudar a função, modificar "1" e "-1" para as eventuais raizes
				// e inserir números até 6 (interface gráfica).
				
				/*
				if (creal (raiz) == 1)
					fprintf (output, "1\n");
				else if (creal (raiz) == 0)
					fprintf (output, "2\n");
				else if (creal (raiz) == 1)
					fprintf(output, "5\n");
				*/
				if (cimag (raiz) == 1)
					fprintf (output, "1\n");
				else if (cimag (raiz) == - 1)	
					fprintf (output, "2\n");
				else
					fprintf (output, "3\n");

				// atualiza z
				z += 0.0 + 1.0 * I;
			}
			l += 1.0 + 0.0 * I;
			z = l;
		}
	}

	fclose (output);
}

int main (int argc, char **argv) 
{	
	float x0, cont, r1, r2, r3;
	
	/*         MÉTODO DO PONTO FIXO      */

	printf ("Encontrar raízes de f(x) através do Método do Ponto Fixo\n"
		" f(x) = exp (x) - 2x²\n\n");
	printf("Insira x0: ");
	scanf ("%f", &x0);

	r1 = pontoFixo (x0, 1);
	if (x0 <= 0)
		x0 += 1;
	else if (x0 >= 2)
		x0 -= 1;

	r2 = pontoFixo (x0, 1);
	while (x0 < 3 && fabs (r2 - r1) < 1) {
		r2 = pontoFixo (x0, 1);
		x0 += 1;
	}
	
	r3 = pontoFixo (x0, 2);
	while (x0 < 3 && (fabs (r3 - r1) < 1 || fabs (r3 - r2) < 1)) {
		r3 = pontoFixo (x0, 2);
		x0 += 1;
	}

	printf ("\nRaiz 1: %f\n", r1);
	printf ("Raiz 2: %f\n", r2);
	printf ("Raiz 3: %f\n", r3);

	/*         MÉTODO DE NEWTON       */

	float complex z0, R1, R2; 
	float real, imag;

	printf ("Encontrar raízes de f(x) através do Método de Newton\n"
		" f(x) = z² + 1\n\n");

	real = 1;
	imag = 1;

	z0 = real + imag * I;
	R1 = newton (z0);

	z0 = real - imag * I;
	R2 = newton (z0);
	
	printf ("\nRaiz 1: %.2f %+.2fi\n", creal (R1), cimag (R1));
	printf ("Raiz 2: %.2f %+.2fi \n", creal (R2), cimag (R2));

	// nº de pontos: 160000 (400 * 400)
	// Para mexer com o domínio, modificar os valores enviados a newton_basis
	// e ps valores de gluOrtho2D
	newton_basins (-200 -200 * I, 200 + 200 * I, 2 * 200 * 2 * 200);

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (3000, 900);
	glutInitWindowPosition (600, 300);
	glutCreateWindow ("Bacias de convergencia");
	
	gluOrtho2D (-200, 200, -200, 200);
	
	glutDisplayFunc (display);

	printf("Aperte esc para fechar.\n");
	
	glutKeyboardFunc (keyboard);	
	glutMainLoop ();

	return 0;
}
