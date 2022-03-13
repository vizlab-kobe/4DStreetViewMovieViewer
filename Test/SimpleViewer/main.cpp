#include <iostream>
#include <fstream>
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/EventListener>
#include <kvs/Slider>
#include <kvs/Label>
#include <kvs/String>
#include <kvs/Directory>
#include <kvs/FileList>
#include <kvs/ffmpeg/MovieObject>
#include <kvs/ffmpeg/SphericalMovieRenderer>


int main( int argc, char** argv )
{
    // Parameters
    kvs::Vec3i dimension{0,0,0};
    kvs::Vec3i position{0,0,0};
    float frame_rate{0};
    kvs::FileList files{};

    // Functions
    auto Read = [&] ( const std::string filename )
    {
        std::ifstream ifs( filename.c_str() );
        if ( !ifs.is_open() )
        {
            std::cerr << "Cannot open " << filename << std::endl;
            return false;
        }

        std::string dirname{""};
        std::string extension{""};
        ifs >> dirname;
        ifs >> extension;
        ifs >> dimension[0] >> dimension[1] >> dimension[2];
        ifs >> position[0] >> position[1] >> position[2];
        ifs >> frame_rate;
        ifs.close();

        kvs::Directory dir( dirname );
        if ( !dir.exists() )
        {
            std::cerr << "Not found " << filename << std::endl;
            return false;
        }

        for ( auto file : dir.fileList() )
        {
            if ( file.extension() == extension )
            {
                files.push_back( file );
            }
        }

        return true;
    };

    auto Index = [&] ()
    {
        const auto& dim = dimension;
        const auto& pos = position;
        return pos.x() + dim.x() * pos.y() + dim.x() * dim.y() * pos.z();
    };

    auto File = [&] ()
    {
        const auto file = files[ Index() ];
        return file.filePath();
    };

    // Read config file
    if ( !Read( argv[1] ) )
    {
        std::cerr << "Cannot read " << std::string( argv[1] ) << std::endl;
        return 1;
    }

    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );

    using Object = kvs::ffmpeg::MovieObject;
    using Renderer = kvs::ffmpeg::SphericalMovieRenderer;
    auto* object = new Object( File() );
    auto* renderer = new Renderer();
    object->setName( "Object" );
    screen.create();

    kvs::Label label( &screen );
    label.anchorToTopLeft();
    label.setFont( { kvs::Font::Sans, 18, kvs::RGBColor::White() } );
    label.screenUpdated( [&]()
    {
        auto index = Object::DownCast( screen.scene()->object( "Object" ) )->currentFrameIndex();
        auto frame = kvs::String::From( index, 6, '0' );
        label.setText( "  File: " + files[ Index() ].fileName() );
        label.addText( "  Dimension: " + dimension.format( " x ","","" ) );
        label.addText( "  Position: " + position.format( ", ","( "," )" ) );
        label.addText( "  Frame: " + frame );
    } );
    label.show();

    kvs::Slider slider( &screen );
    slider.anchorToBottomRight();
    slider.setFont( { kvs::Font::Sans, 18, kvs::RGBColor::White() } );
    slider.setCaption( "Frame: " + kvs::String::From( 0, 6, '0' ) );
    slider.setRange( 0, object->numberOfFrames() - 1 );
    slider.setValue( 0 );
    slider.sliderMoved( [&] ()
    {
        auto index = kvs::Math::Round( slider.value() );
        auto frame = kvs::String::From( index, 6, '0' );
        slider.setValue( index );
        slider.setCaption( "Frame: " + frame );
    } );
    slider.sliderPressed( [&]()
    {
        renderer->pause();
    } );
    slider.sliderReleased( [&]()
    {
        auto index = kvs::Math::Round( slider.value() );
        object->jumpToFrame( index );
        renderer->play();
        screen.redraw();
        renderer->pause();
    } );
    slider.show();

    const int interval = 1000 / frame_rate; // msec
    kvs::EventListener event;
    event.keyPressEvent( [&] ( kvs::KeyEvent* e )
    {
        auto ReplaceObject = [&] ()
        {
            auto index = Object::DownCast( screen.scene()->object( "Object" ) )->currentFrameIndex();
            auto* o = new Object( File() );
            o->setName( "Object" );
            o->jumpToFrame( index );
            screen.scene()->replaceObject( "Object", o );
        };

        switch ( e->key() )
        {
        case kvs::Key::Left:
        {
            position += kvs::Vec3i{ -1, 0, 0 };
            position.x() = kvs::Math::Max( position.x(), 0 );
            ReplaceObject();
            break;
        }
        case kvs::Key::Right:
        {
            position += kvs::Vec3i{ +1, 0, 0 };
            position.x() = kvs::Math::Min( position.x(), dimension.x() - 1 );
            ReplaceObject();
            break;
        }
        case kvs::Key::Up:
        {
            position += kvs::Vec3i{ 0, +1, 0 };
            position.y() = kvs::Math::Min( position.y(), dimension.y() - 1 );
            ReplaceObject();
            break;
        }
        case kvs::Key::Down:
        {
            position += kvs::Vec3i{ 0, -1, 0 };
            position.y() = kvs::Math::Max( position.x(), 0 );
            ReplaceObject();
            break;
        }

        case kvs::Key::s: { renderer->stop(); break; }
        case kvs::Key::Space:
        {
            if ( renderer->isPaused() ) { renderer->play(); }
            else if ( renderer->isPlaying() ) { renderer->pause(); }
            else { renderer->play(); }
            break;
        }
        default: break;
        }
    } );
    event.timerEvent( [&] ( kvs::TimeEvent* e )
    {
        if ( !object->isLastFrame() ) { screen.redraw(); kvs::OpenGL::Flush(); }
        if ( renderer->isPlaying() )
        {
            auto index = Object::DownCast( screen.scene()->object( "Object" ) )->currentFrameIndex();
            auto frame = kvs::String::From( index, 6, '0' );
            slider.setValue( index );
            slider.setCaption( "Frame: " + frame );
            screen.redraw();
        }
    }, interval );
    screen.addEvent( &event );

    screen.registerObject( object, renderer );
    screen.show();
    
    return app.run();
}