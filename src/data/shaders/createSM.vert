uniform mat4 MVPmatrix; // model-view-projection matrix of light source

void main() {
	// Project vertex into light - NDC
	gl_Position = MVPmatrix * gl_Vertex;
}