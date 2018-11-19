class RegistrationContext:
    def __init__(self, strategy):
        self._strategy = strategy

    def Registration(self, szeReader,  szeLMReader, mriLMReader, mriReader, wrlLMReader):
        self._strategy.SurfaceXRayRegistration(szeLMReader, wrlLMReader, szeReader)
        self._strategy.MRIXRayRegistration(mriLMReader, wrlLMReader, mriReader)