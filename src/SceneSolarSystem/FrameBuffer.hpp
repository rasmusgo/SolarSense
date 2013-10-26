#ifndef __FRAMEBUFFER_H_
#define __FRAMEBUFFER_H_

#include "Commons.hpp"
#include <vector>

class FramebufferObject {
protected:
	std::vector<GLuint> textureIds;
	GLuint fboId, rboId;

	bool depthBuffer;
	int width;
	int height;

	std::vector<GLuint> attachments;

	void createFramebufferObjects();

}