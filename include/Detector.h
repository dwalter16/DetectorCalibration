#ifndef DETECTOR_H
#define DETECTOR_H
#include <string>
#include <TVector3.h>

/**
 * Abstract detector class containing information on position, orientation, dead
 * layer thickness and so on. Used as base class for more advanced detectors
 * with multiple active segments.
 *
 * The unit of the spatial coordinates should be millimeters and the dead layer
 * thickness should be given in nanometers.
 */
class Detector {
  protected:
    TVector3 position; /**< The x,y,z position of the detector.*/
    TVector3 normal;   /**< The normal vector to the detector surface.*/
    TVector3 orientation;
    double deadLayer;  /**< Thickness of the detector dead layer*/
    
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
    
    void SetPosition(TVector3 &pos);
    void SetPosition(double x, double y, double z);
    TVector3 GetPosition();
    
    /**
     * Define the direction normal to the active detector surface. The input is
     * automatically normalised.
     */
    void SetNormal(TVector3 &n);
    void SetNormal(double nx, double ny, double nz);
    TVector3 GetNormal();
    
    /**
     * Provide the spatial orientation of the detector. The interpretation of
     * this vector will be dependent on the specific detector class, and has no
     * meaning for rotationally symmetric detectors (i.e. surface barrier
     * detectors).
     * @param ori The orientation vector.
     */
    void SetOrientation(TVector3 &ori);
    void SetOrientation(double ox, double oy, double oz);

    /**
     * Get the normalised orientation vector.
     */
    TVector3 GetOrientation();
    
    /**
     * Set the dead layer thickness in nm.
     */     
    void SetDeadLayer(double dead_layer);
    double GetDeadLayer();

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
     * Find the total solid angle subtended by the detector as viewed from the
     * given position.
     * @param pos The position with respect to which to calculate the solid
     *            angle.
     * @return The calculated solid angle in steradians.
     */    
    virtual double GetSolidAngle(TVector3 &pos);
};
#endif
