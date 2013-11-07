
CREATE SCHEMA IF NOT EXISTS `TuftsTissueTracker` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
SHOW WARNINGS;
USE `TuftsTissueTracker` ;

-- -----------------------------------------------------
-- Table `Project`
-- -----------------------------------------------------

CREATE  TABLE IF NOT EXISTS `Project` (
  `idProject` INT NOT NULL AUTO_INCREMENT ,
  `spacingX` DOUBLE NOT NULL ,
  `spacingY` DOUBLE NOT NULL ,
  `spacingZ` DOUBLE NOT NULL ,
  `name` VARCHAR(45) NOT NULL ,
  `timeDelta` DOUBLE NOT NULL ,
  `sizeX` INT NOT NULL ,
  `sizeY` INT NOT NULL ,
  `sizeZ` INT NOT NULL ,
  `workingDirectory` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`idProject`) )
ENGINE = InnoDB;

-- -----------------------------------------------------
-- Table `Frame`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Frame` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`) ,
  CONSTRAINT `Project`
    FOREIGN KEY (`idProject` )
    REFERENCES `Project` (`idProject` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;



-- -----------------------------------------------------
-- Table `RawImages`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `RawImages` (
	`idProject` INT NOT NULL ,  
	`t` INT NOT NULL ,
	`fileName` VARCHAR(45) NOT NULL ,
	PRIMARY KEY (`idProject`,`t`) ,
	CONSTRAINT `RawImages_Frame`
    FOREIGN KEY (`idProject`,`t` )
    REFERENCES `Frame` (`idProject`,`t`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;


-- -----------------------------------------------------
-- Table `SurfaceSegmentedImages`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `SurfaceSegmentedImages` (
	`idProject` INT NOT NULL ,  
	`t` INT NOT NULL ,
	`fileName` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`idProject`,`t`) ,
  CONSTRAINT `SurfaceSegmentedImages_Frame`
    FOREIGN KEY (`idProject`,`t` )
    REFERENCES `Frame` (`idProject`,`t`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
-- -----------------------------------------------------
-- Table `DiffusedImages`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `DiffusedImages` (
	`idProject` INT NOT NULL ,
	`t` INT NOT NULL ,
	`fileName` VARCHAR(45) NOT NULL ,
	PRIMARY KEY (`idProject`,`t`) ,
	CONSTRAINT `DiffusedImages_Frame`
    FOREIGN KEY (`idProject`,`t` )
    REFERENCES `Frame` (`idProject`,`t`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `PlatenessImages`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `PlatenessImages` (
	`idProject` INT NOT NULL ,
	`t` INT NOT NULL ,
	`fileName` VARCHAR(45) NOT NULL ,
	PRIMARY KEY (`idProject`,`t`) ,
	CONSTRAINT `PlatenessImages_Frame`
    FOREIGN KEY (`idProject`,`t` )
    REFERENCES `Frame` (`idProject`,`t`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `OrientationImages`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `OrientationImages` (
	`idProject` INT NOT NULL ,
	`t` INT NOT NULL ,
	`fileName` VARCHAR(45) NOT NULL ,
	PRIMARY KEY (`idProject`,`t`) ,
	CONSTRAINT `OrientationImages_Frame`
    FOREIGN KEY (`idProject`,`t` )
    REFERENCES `Frame` (`idProject`,`t`  )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `VertexnessImages`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `VertexnessImages` (
	`idProject` INT NOT NULL ,
	`t` INT NOT NULL ,
	`fileName` VARCHAR(45) NOT NULL ,
	PRIMARY KEY (`idProject`,`t`) ,
	CONSTRAINT `VertexnessImages_Frame`
    FOREIGN KEY (`idProject`,`t` )
    REFERENCES `Frame` (`idProject`,`t`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `VertexLocations`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `VertexLocations` (
	`idProject` INT NOT NULL ,
	`t` INT NOT NULL ,
	`pointX` INT NOT NULL ,
	`pointY` INT NOT NULL ,
	`pointZ` INT NOT NULL ,
	PRIMARY KEY (`idProject`,`t`, `pointX`, `pointY`, `pointZ`) ,
	CONSTRAINT `VertexLocations_Frame`
    FOREIGN KEY (`idProject`,`t`)
    REFERENCES `Frame` (`idProject`,`t`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `Tissue`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Tissue` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`) ,
  CONSTRAINT `Tissue_Frame`
    FOREIGN KEY (`idProject`,`t` )
    REFERENCES `Frame` ( `idProject`,`t` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `MembranePoint`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `MembranePoint` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idMembranePoint` INT NOT NULL ,
  `posX` DOUBLE NOT NULL ,
  `posY` DOUBLE NOT NULL ,
  `posZ` DOUBLE NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idMembranePoint`) ,
  CONSTRAINT `MembranePoint_Tissue`
    FOREIGN KEY ( `idProject` , `t` , `idTissue` )
    REFERENCES `Tissue` (`idProject` ,`t` , `idTissue` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `MembranePoint_linkedTo_MembranePoint`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `MembranePoint_linkedTo_MembranePoint` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idMembranePoint1` INT NOT NULL ,
  `idMembranePoint2` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idMembranePoint1`, `idMembranePoint2`) ,
  CONSTRAINT `MembranePoint_linkedTo_MembranePoint_source`
    FOREIGN KEY (`idProject` , `t` , `idTissue` , `idMembranePoint1` )
    REFERENCES `MembranePoint` (`idProject` , `t` , `idTissue` , `idMembranePoint` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `MembranePoint_linkedTo_MembranePoint_target`
    FOREIGN KEY ( `idProject` , `t`, `idTissue`,`idMembranePoint2`   )
    REFERENCES `MembranePoint` ( `idProject` , `t`,`idTissue`,`idMembranePoint`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


SHOW WARNINGS;


-- -----------------------------------------------------
-- Table `Cell`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Cell` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idCell` INT NOT NULL ,
  `posX` DOUBLE NOT NULL ,
  `posY` DOUBLE NOT NULL ,
  `posZ` DOUBLE NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idCell`) ,
  CONSTRAINT `Cell_Tissue`
    FOREIGN KEY (`idProject` , `t` , `idTissue` )
    REFERENCES `Tissue` (`idProject` ,`t` , `idTissue` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;


-- -----------------------------------------------------
-- Table `Cell_neighbor_of_Cell`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Cell_neighbor_of_Cell` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idCell1` INT NOT NULL ,
  `idCell2` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idCell1`, `idCell2`) ,
  CONSTRAINT `Cell_neighbor_of_Cell_source`
    FOREIGN KEY (`idProject` , `t` , `idTissue` , `idCell1` )
    REFERENCES `Cell` (`idProject` , `t` , `idTissue` , `idCell`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `Cell_neighbor_of_Cell_target`
    FOREIGN KEY (`idProject` , `t` ,  `idTissue`,`idCell2`  )
    REFERENCES `Cell` (  `idProject` ,`t` , `idTissue`,`idCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;


-- -----------------------------------------------------
-- Table `Cell_has_MembranePoint`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Cell_has_MembranePoint` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idMembranePoint` INT NOT NULL ,
  `idCell` INT NOT NULL ,
  `order` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idMembranePoint`, `idCell`) ,
  CONSTRAINT `Cell_has_MembranePoint_MembranePoint`
    FOREIGN KEY (`idProject` ,`t` ,  `idTissue` , `idMembranePoint` )
    REFERENCES `MembranePoint` (`idProject` ,`t` ,  `idTissue` , `idMembranePoint` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `Cell_has_MembranePoint_Cell`
    FOREIGN KEY ( `idProject` ,`t` ,  `idTissue`,`idCell` )
    REFERENCES `Cell` (`idProject` , `t` , `idTissue`,`idCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;






-- -----------------------------------------------------
-- Table `CellAreas`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `CellAreas` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idCell` INT NOT NULL ,
  `area` DOUBLE NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idCell`) ,
  CONSTRAINT `CellAreas_Cell`
    FOREIGN KEY (`idProject` ,`t` , `idTissue` , `idCell` )
    REFERENCES `Cell` (`idProject` ,`t` , `idTissue` , `idCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

USE `TuftsTissueTracker` ;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `Ellipse`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Ellipse` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idCell` INT NOT NULL ,
  `majorR` DOUBLE NULL ,
  `minorR` DOUBLE NULL ,
  `rotR` DOUBLE NULL ,
  `posX` DOUBLE NULL ,
  `posY` DOUBLE NULL ,
  `posZ` DOUBLE NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idCell`) ,
  CONSTRAINT `cell`
    FOREIGN KEY (`idProject` ,`t` , `idTissue` , `idCell` )
    REFERENCES `Cell` (`idProject` ,`t`,`idTissue` , `idCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;



-- -----------------------------------------------------
-- Table `TrackedMembranePoint`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TrackedMembranePoint` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedMembranePoint` INT NOT NULL ,
  `posX` DOUBLE NOT NULL ,
  `posY` DOUBLE NOT NULL ,
  `posZ` DOUBLE NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedMembranePoint`) ,
  CONSTRAINT `TrackedMembranePoint_Tissue`
    FOREIGN KEY (`idProject` , `t` , `idTissue` )
    REFERENCES `Tissue` (`idProject` , `t` , `idTissue` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `TrackedMembranePoint_linkedTo_TrackedMembranePoint`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TrackedMembranePoint_linkedTo_TrackedMembranePoint` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedMembranePoint1` INT NOT NULL ,
  `idTrackedMembranePoint2` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedMembranePoint1`, `idTrackedMembranePoint2`) ,
  CONSTRAINT `TrackedMembranePoint_linkedTo_TrackedMembranePoint_source`
    FOREIGN KEY (`idProject` , `t` , `idTissue` , `idTrackedMembranePoint2` )
    REFERENCES `TrackedMembranePoint` (`idProject` , `t` , `idTissue` , `idTrackedMembranePoint` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `TrackedMembranePoint_linkedTo_TrackedMembranePoint_target`
    FOREIGN KEY (`idProject` , `t` , `idTissue` , `idTrackedMembranePoint2` )
    REFERENCES `TrackedMembranePoint` (`idProject` , `t` , `idTissue` , `idTrackedMembranePoint` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;


-- -----------------------------------------------------
-- Table `TrackedCell`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TrackedCell` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedCell` INT NOT NULL ,
  `idCell` INT NOT NULL ,
  `trackID` INT NOT NULL ,
  `posX` DOUBLE NOT NULL ,
  `posY` DOUBLE NOT NULL ,
  `posZ` DOUBLE NOT NULL ,
  `velX` DOUBLE NOT NULL ,
  `velY` DOUBLE NOT NULL ,
  `velZ` DOUBLE NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedCell`) ,
  CONSTRAINT `TrackedCell Tissue`
    FOREIGN KEY (`idProject` , `t` ,`idTissue` )
    REFERENCES `Tissue` (`idProject` , `t` ,`idTissue` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `TrackedCell_neighbor_of_TrackedCell`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TrackedCell_neighbor_of_TrackedCell` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedCell1` INT NOT NULL ,
  `idTrackedCell2` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`,  `idTrackedCell1`,`idTrackedCell2`) ,
  CONSTRAINT `TrackedCell_neighbor_of_TrackedCell_source`
    FOREIGN KEY ( `idProject` ,`t` , `idTissue`,`idTrackedCell1`  )
    REFERENCES `TrackedCell` (`idProject` , `t` ,  `idTissue`,`idTrackedCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `TrackedCell_neighbor_of_TrackedCell_target`
    FOREIGN KEY (  `idProject` ,`t` , `idTissue`,`idTrackedCell2`)
    REFERENCES `TrackedCell` ( `idProject` ,`t` ,`idTissue`,`idTrackedCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `TrackedCell_has_TrackedMembranePoint`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TrackedCell_has_TrackedMembranePoint` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedCell` INT NOT NULL ,
  `idTrackedMembranePoint` INT NOT NULL ,
  `order` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedCell`, `idTrackedMembranePoint`) ,
  CONSTRAINT `TrackedCell_has_TrackedMembranePoint_TrackedCell`
    FOREIGN KEY (`idProject` , `t` , `idTissue` , `idTrackedCell` )
    REFERENCES `TrackedCell` (`idProject` , `t` , `idTissue` , `idTrackedCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `TrackedCell_has_TrackedMembranePoint_TrackedMembranePoint`
    FOREIGN KEY ( `idProject` , `t` , `idTissue`,`idTrackedMembranePoint` )
    REFERENCES `TrackedMembranePoint` (`idProject` , `t` , `idTissue`,`idTrackedMembranePoint`  )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;



-- -----------------------------------------------------
-- Table `TrackedEllipse`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TrackedEllipse` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedCell` INT NOT NULL ,
  `majorR` DOUBLE NULL ,
  `minorR` DOUBLE NULL ,
  `rotR` DOUBLE NULL ,
  `posX` DOUBLE NULL ,
  `posY` DOUBLE NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedCell`) ,
  CONSTRAINT `TrackedEllipse_TrackedCell`
    FOREIGN KEY (`idProject` , `t` , `idTissue` , `idTrackedCell` )
    REFERENCES `TrackedCell` (`idProject` , `t` , `idTissue` , `idTrackedCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;


-- -----------------------------------------------------
-- Table `TrackedDomain`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TrackedDomain` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedCell` INT NOT NULL ,
  `order` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedCell`, `order`) ,
  CONSTRAINT `TrackedDomain_TrackedCell`
    FOREIGN KEY (`idProject` , `t` , `idTissue`, `idTrackedCell` )
    REFERENCES `TrackedCell` (`idProject` , `t` ,  `idTissue`,`idTrackedCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `TrackedDomain_has_TrackedCell`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TrackedDomain_has_TrackedCell` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedCell` INT NOT NULL ,
  `idTrackedNucleousCell` INT NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedCell`, `idTrackedNucleousCell`) ,
  CONSTRAINT `TrackedDomain_has_TrackedCell_TrackedDomain`
    FOREIGN KEY (`idProject` , `t` , `idTissue` , `idTrackedNucleousCell` )
    REFERENCES `TrackedDomain` (`idProject` ,`t` , `idTissue` , `idTrackedCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `TrackedDomain_has_TrackedCell_TrackedDomain_TrackedCell`
    FOREIGN KEY (`idProject` , `t` , `idTissue` , `idTrackedCell` )
    REFERENCES `TrackedCell` (`idProject` , `t` , `idTissue` , `idTrackedCell` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;



-- -----------------------------------------------------
-- Table `TissueStrainRate`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `TissueStrainRate` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedCell` INT NOT NULL ,
  `order` INT NOT NULL ,
  `data` TINYBLOB NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedCell`, `order`) ,
  CONSTRAINT `TissueStrainRate_TrackedDomain`
    FOREIGN KEY ( `idProject` , `t` , `idTissue` ,`idTrackedCell` , `order` )
    REFERENCES `TrackedDomain` (`idProject` ,`t` ,  `idTissue` ,`idTrackedCell` ,  `order` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `IntercalationStrainRates`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `IntercalationStrainRates` (
  `idProject` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `idTrackedCell` INT NOT NULL ,
  `order` INT NOT NULL ,
  `data` TINYBLOB NOT NULL ,
  PRIMARY KEY (`idProject`, `t`, `idTissue`, `idTrackedCell`, `order`) ,
  CONSTRAINT `IntercalationStrainRates_TrackedDomain`
    FOREIGN KEY (`idProject` , `t` ,  `idTissue` ,`idTrackedCell` , `order` )
    REFERENCES `TrackedDomain` (`idProject` , `t` ,  `idTissue` , `idTrackedCell` , `order` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `CellStrainRates`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `CellStrainRates` (
  `idTrackedCell` INT NOT NULL ,
  `t` INT NOT NULL ,
  `idProject` INT NOT NULL ,
  `idTissue` INT NOT NULL ,
  `order` INT NOT NULL ,
  `data` TINYBLOB NULL ,
  PRIMARY KEY (`idTrackedCell`, `t`, `idProject`, `idTissue`, `order`) ,
  CONSTRAINT `CellStrainRates_TrackedDomain`
    FOREIGN KEY (`idProject` ,`t` ,   `idTissue` ,`idTrackedCell` , `order` )
    REFERENCES `TrackedDomain` (`idProject` , `t` ,  `idTissue` ,`idTrackedCell` , `order` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;