package de.hrw.xilab.spring.repository;

import de.hrw.xilab.spring.model.wrapper.DeviceWrapper;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Repository
public interface DeviceRepository extends CrudRepository<DeviceWrapper, Integer> {
    List<DeviceWrapper> findAll();

    Optional<DeviceWrapper> findByUuid(String uuid);

    @Override
    default <S extends DeviceWrapper> Iterable<S> saveAll(Iterable<S> entities){
        return new ArrayList<>();
    }

}
