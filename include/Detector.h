#ifndef DETECTOR_H
#define DETECTOR_H
#include <string>
#include <TVector3.h>

/**
 * Abstract detector class containing information on position, orientation, dead
 * layer thickness and so on. Used as base class for more advanced detectors
 * with multiple active segments.
 *
 * Instances of the Detector class are initialised with a standard position and
 * orientation, but can later be moved and rotated.
 *
 * The unit of the spatial coordinates should be millimeters and the dead layer
 * thickness should be given in nanometers.
 */
class Detector {
  protected:
    TVector3 position;    /**< The x,y,z position of the detector.*/
    TVector3 normal;      /**< The normal vector to the detector surface.*/
    TVector3 orientation; /**< A vector defining the orientation of the detector.*/
    double deadLayer;     /**< Thickness of the detector dead layer*/
    
  public:
    Detector() = default;
    virtual ~Detector() = default;
    
    /**
     * Factory method to create instances of the various detector classes.
     * @param type The name of the detector type to be created. Valid types
     *             include "YY1".
     * @return A new instance of the requested detector class.
     */
    static Detector * Create(std::string type);
    
    /**
     * Set a new position for the detector. See the documentation for the
     * specific detector class to learn how the detector geometry is related to
     * the position.
     */
    virtual void SetPosition(TVector3 &pos);
    virtual void SetPosition(double x, double y, double z);
    void SetX(double x);
    void SetY(double y);
    void SetZ(double z);
    TVector3 GetPosition();
    
    /**
     * Get a unit vector normal to the detector surface.
     */
    TVector3 GetNormal();
    
    /**
     * Get the normalised orientation vector. How the orientation is defined
     * depends on the individual detector implementation.
     */
    TVector3 GetOrientation();
    
    /**
     * Rotate the detector around the x-axis.
     * @param angle Rotation angle in degrees.
     */
    virtual void RotateX(double angle);
 
     /**
     * Rotate the detector around the y-axis.
     * @param angle Rotation angle in degrees.
     */   
    virtual void RotateY(double angle);
    
    /**
     * Rotate the detector around the z-axis.
     * @param angle Rotation angle in degrees.
     */
    virtual void RotateZ(double angle);
    
    /**
     * Rotate the detector around an arbitrary axis.
     * @param angle Rotation angle in degrees.
     * @param axis Vector specifying the axis of rotation.
     */
    virtual void Rotate(double angle, TVector3 &axis);
    
    /**
     * Set the dead layer thickness in nm.
     */     
    void SetDeadLayer(double dead_layer);
    double GetDeadLayer();

    /**
     * Find the total solid angle subtended by the detector as viewed from the
     * given position.
     * @param pos The position with respect to which to calculate the solid
     *            angle.
     * @return The calculated solid angle in steradians.
     */    
    virtual double GetSolidAngle(TVector3 &pos);

    //void SetNSegments(int n_segments);
    virtual int GetNSegments() = 0;
    
    /**
     * Get the global position of a segment.
     * @param i The segment index.
     * @return The global position of the segment.
     */
    virtual TVector3 GetSegPosition(int i) = 0;
    
    /**
     * Find the solid angle subtended by a segment as viewed from the given
     * position.
     * @param i The segment index.
     * @param pos The position with respect to which to calculate the solid
     *            angle.
     * @return The calculated solid angle in steradians.
     */
    virtual double GetSegSolidAngle(int i, TVector3 &pos) = 0;
    
    /**
     * Get the effective thickness of the dead layer for a given detector segment/strip.
     * @param i is the strip number with i=0 being the inner most strip
     * @param pos is the TVector pointing to the calibration source location
     */
    virtual double GetEffectiveThickness(int i, TVector3 &pos);
};
#endif
