#include <kvs/CommandLine>
#include <kvs/ColorImage>
#include <kvs/CubicImage>
#include <kvs/SphericalImage>


class Input
{
private:
    kvs::CommandLine m_commandline;

public:
    std::string top;
    std::string left;
    std::string right;
    std::string bottom;
    std::string back;
    std::string front;
    std::string output;

    Input( int argc, char** argv )
    {
        m_commandline = kvs::CommandLine( argc, argv );
        m_commandline.addHelpOption();
        m_commandline.addOption( "top", "Top image name", 1, true );
        m_commandline.addOption( "left", "Top image name", 1, true );
        m_commandline.addOption( "right", "Top image name", 1, true );
        m_commandline.addOption( "bottom", "Top image name", 1, true );
        m_commandline.addOption( "back", "Top image name", 1, true );
        m_commandline.addOption( "front", "Top image name", 1, true );
        m_commandline.addOption( "output", "Output image name", 1, true );
     }

     bool parse()
     {
        if ( !m_commandline.parse() ) { return false; }

        top = m_commandline.optionValue<std::string>("top");
        left = m_commandline.optionValue<std::string>("left");
        right = m_commandline.optionValue<std::string>("right");
        bottom = m_commandline.optionValue<std::string>("bottom");
        back = m_commandline.optionValue<std::string>("back");
        front = m_commandline.optionValue<std::string>("front");
        output = m_commandline.optionValue<std::string>("output");

        return true;
     }
};

int main( int argc, char** argv )
{
    Input input( argc, argv );
    if ( !input.parse() ) { return 1; }

    kvs::CubicImage cube;
    cube.setLeftImage( kvs::ColorImage( input.left ) );
    cube.setRightImage( kvs::ColorImage( input.right ) );
    cube.setTopImage( kvs::ColorImage( input.top ) );
    cube.setBottomImage( kvs::ColorImage( input.bottom ) );
    cube.setFrontImage( kvs::ColorImage( input.front ) );
    cube.setBackImage( kvs::ColorImage( input.back ) );

    kvs::SphericalImage sphe( cube );
    sphe.write( input.output ); 

    return 0;
}
