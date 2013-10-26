FrameBuffer::FrameBuffer(const std::string& name, float radius, float orbRadius) : Planet(name, radius, orbRadius), time(0.0f) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get("earthShader");
}

FrameBuffer::~FrameBuffer(){

}

FrameBuffer::createFramebufferObject(){

}