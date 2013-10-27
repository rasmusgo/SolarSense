#ifndef __FRAMEBUFFER_H_
#define __FRAMEBUFFER_H_

#include "Commons.hpp"
#define GL_DEPTH_BUFFER 0x8223

class FramebufferObject {
	protected:
		std::vector<GLuint> textureIds;
		GLuint fboId, rboId;

		bool depthBuffer;
		int width;
		int height;

		FramebufferObject(){
			depthBuffer = false;
		}

		std::vector<GLuint> attachments;

		void createFramebufferObject(){
			//create a framebuffer object
            glGenFramebuffersEXT(1, &fboId);
			glBindFramebufferEXT(GL_FRAMEBUFFER, fboId);

			//attach the texture to FBO color attachment point
			for(unsigned int i = 0; i < textureIds.size(); i++){
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, textureIds[i], 0);
			}

			if(depthBuffer){
				glGenRenderbuffersEXT(1, &rboId);
				glBindFramebufferEXT(GL_RENDERBUFFER, rboId);
				glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
			}

			//check FBO status
			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
			if(status != GL_FRAMEBUFFER_COMPLETE){
				throw "Framebuffer is not complete, attachments could be wrong!";
			}

			glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		}

	public:
		FramebufferObject(std::vector<GLuint> attachments, std::vector<Texture*> textures, bool useDepthBuffer = false){
			this->attachments = attachments;
			for(unsigned int i=0; i < textures.size(); i++){
				textureIds.push_back(textures[i]->getSlot());
			}

			depthBuffer = useDepthBuffer;
			if(useDepthBuffer && textures.empty()){
				throw "No Color Buffer given";
			}
			else {
				width = textures[0]->getWidth();
				height = textures[0]->getHeight();
			}

			createFramebufferObject();
		}

		virtual ~FramebufferObject(){
			// switch back to window-system probided framebuffer
			glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

			glDeleteFramebuffersEXT(1, &fboId);

			if(depthBuffer) {
				glDeleteRenderbuffersEXT(1, &rboId);
			}
		}

		void bind()
        {
            glBindFramebufferEXT(GL_FRAMEBUFFER, fboId);

			// multiple color buffers must be activated explicitly
			std::vector<GLuint> drawBuffers;
			for(unsigned int i=0; i<attachments.size(); i++){
				if(attachments[i] != GL_DEPTH_BUFFER){
					drawBuffers.push_back(attachments[i]);
				}
			}
            glDrawBuffers(drawBuffers.size(), drawBuffers.data());
		}

		void unbind()
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		}

        GLuint getID() { return fboId; }


};

#endif /*__FRAMEBUFFER_H_*/
