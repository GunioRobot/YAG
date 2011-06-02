#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "YAGController.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class YAGSampleApp : public AppBasic {
  public:
    void prepareSettings(Settings* settings);
	void setup();
	void mouseDown( MouseEvent event );	
	void keyDown( KeyEvent event );
	void update();
	void draw();
    
    YAGController *m_yag;
    
    float   myFloat;
    Vec2f   myVec2f;
    Vec3f   myVec3f;
    int     myInt;
    
    gl::Texture m_textureBg;
};

void YAGSampleApp::prepareSettings(Settings* settings) {
	settings->setWindowSize(1024, 768);
}

void YAGSampleApp::setup()
{
    myInt = 1;
    myFloat = 1.0f;
    myVec2f = Vec2f::one();
	myVec3f = Vec3f::one();
    m_yag = new YAGController(this);
    
    m_yag->addControl(new YAGControl(m_yag, &myInt,	"My int", 0, 5, Vec2f(10,110)));
	m_yag->addControl(new YAGControl(m_yag, &myFloat, "My float", 0, 10, Vec2f(10,160)));
    m_yag->addControl(new YAGControl(m_yag, &myVec2f, "My Vec2f", 0, 10, Vec2f(10,210)));
	m_yag->addControl(new YAGControl(m_yag, &myVec3f, "My Vec3f", 0, 10, Vec2f(10,280)));
    
	m_textureBg = gl::Texture( loadImage( loadResource( RES_BG ) ) );

}

void YAGSampleApp::mouseDown( MouseEvent event )
{
}

void YAGSampleApp::keyDown( KeyEvent event )
{
	if (event.getChar() == 's')
		m_yag->save("yag.settings.txt");
	
	if (event.getChar() == 'l')
		m_yag->load("yag.settings.txt");

}

void YAGSampleApp::update()
{
    m_yag->update();
}

void YAGSampleApp::draw()
{
	gl::clear( Color( 0.16, 0.15, 0.21 ) ); 
	gl::enableAlphaBlending();
	
	gl::draw(m_textureBg);
    
	gl::pushMatrices();
	
	gl::translate( Vec3f(200, getWindowHeight(), 0) );
	gl::scale( Vec3f(25, 25, 1) );
	
	gl::color( ColorA(0.7f, 0.9f, 0.3f, 1.0f) );
	gl::drawSolidRect( Rectf(0, 0, 1, -myInt) );
	
	gl::color( ColorA(0.0f, 1.0f, 1.0f, 1.0f) );
	gl::drawSolidRect( Rectf(2, 0, 3, -myFloat) );
	
	gl::color( ColorA(1.0f, 0.25f, 0.5f, 1.0f) );
	gl::drawSolidRect( Rectf(4, 0, 5, -myVec2f.x) );
	gl::drawSolidRect( Rectf(5.1, 0, 6.1, -myVec2f.y) );
	
	
	gl::color( ColorA(0.4f, 0.2f, 0.6f, 1.0f) );
	gl::drawSolidRect( Rectf(7.1, 0, 8.1, -myVec3f.x) );
	gl::drawSolidRect( Rectf(8.2, 0, 9.2, -myVec3f.y) );
	gl::drawSolidRect( Rectf(9.3, 0, 10.3, -myVec3f.z) );
	
	gl::popMatrices();
	
	
    m_yag->draw();
}


CINDER_APP_BASIC( YAGSampleApp, RendererGl )
