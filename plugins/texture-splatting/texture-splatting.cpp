#include "texture-splatting.h"
#include <QFileDialog>
 
void TextureSplatting::onPluginLoad()
{
	// VS
    QString vs_src = "#version 330 core\nin vec3 vertex; out vec2 vtexCoord; uniform mat4 modelViewProjectionMatrix; uniform float radius; void main() {	gl_Position    = modelViewProjectionMatrix * vec4(vertex,1.0); 	vtexCoord = (4.0/radius)*(vertex.xy+vertex.zx); }";
    vs = new QGLShader(QGLShader::Vertex, this);
    vs->compileSourceCode(vs_src);

	// FS
    QString fs_src = "#version 330 core\nuniform sampler2D sampler0; uniform sampler2D sampler1; uniform sampler2D sampler2; in vec2 vtexCoord; out vec4 fragColor; void main() { fragColor =  mix(texture(sampler0, vtexCoord), texture(sampler1, vtexCoord), texture(sampler2, vtexCoord)); }";
    fs = new QGLShader(QGLShader::Fragment, this);
    fs->compileSourceCode(fs_src);

	// Program  
  	program = new QGLShaderProgram(this);
    program->addShader(vs);
	program->addShader(fs);
	program->link();

	// Load Texture 1
	glActiveTexture(GL_TEXTURE0);
	QString filename = QFileDialog::getOpenFileName(0, "Open Image", "/assig/grau-g/Textures", "Image file (*.png *.jpg)");	
	QImage img0(filename);	
	QImage im0 = QGLWidget::convertToGLFormat(img0);
	glwidget()->makeCurrent();
	glGenTextures( 1, &textureId0);
	glBindTexture(GL_TEXTURE_2D, textureId0);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im0.width(), im0.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im0.bits());
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture(GL_TEXTURE_2D, 0);

	// Load Texture 2
	glActiveTexture(GL_TEXTURE1);
	QString filename2 = QFileDialog::getOpenFileName(0, "Open Image 2", "/assig/grau-g/Textures", "Image file (*.png *.jpg)");	
	QImage img1(filename2);	
	QImage im1 = QGLWidget::convertToGLFormat(img1);
	glwidget()->makeCurrent();
	glGenTextures( 1, &textureId1);
	glBindTexture(GL_TEXTURE_2D, textureId1);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im1.width(), im1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im1.bits());
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture(GL_TEXTURE_2D, 0);

	// Load Texture 2
	glActiveTexture(GL_TEXTURE1);
	QString filename3 = QFileDialog::getOpenFileName(0, "Open Image 3", "/assig/grau-g/Textures", "Image file (*.png *.jpg)");	
	QImage img2(filename3);	
	QImage im2 = QGLWidget::convertToGLFormat(img2);
	glwidget()->makeCurrent();
	glGenTextures( 1, &textureId2);
	glBindTexture(GL_TEXTURE_2D, textureId2);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im2.width(), im2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im2.bits());
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureSplatting::preFrame() 
{
    // bind shader and define uniforms
    program->bind();
    program->setUniformValue("sampler0", 0);  // texture unit del primer sampler 
    program->setUniformValue("sampler1", 1);  // texture unit del segon  sampler 
    program->setUniformValue("sampler2", 2);  // texture unit del segon  sampler 
    program->setUniformValue("radius", float(scene()->boundingBox().radius()));  // radi d'una esfera que engloba l'escena
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP); 

    // bind textures
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId1);
	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureId2);
}

void TextureSplatting::postFrame() 
{
    // unbind shader
    program->release();
   	// unbind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(multitex, Multitex)   // plugin name, plugin class
#endif



